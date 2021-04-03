/* 
 * indexer.c
 * 
 * The indexer reads the files produced by the crawler, 
 * builds an index, and writes that index to a file.
 * 
 * Usage: ./indexer pageDirectory indexFilename
 * 
 * Args:
 *    pageDirectory is the directory in which files produced by the crawler are saved
 *    indexFilename is the filename to save the index
 * 
 * Exit status:
 *    1: incorrect arguments
 *    2: if file pointer is NULL when calling savaIndexfile
 *
 * Assumptions:
 *    1. pageDirectory has files named 1, 2, 3, ..., without gaps
 *    2. The content of files in pageDirectory follows the following format:
 *       the first line is the url
 *       the second line is the depth at which crawler finds it
 *       the rest of the file is the html retrieved from the url
 *
 * Zitong Wu, Feb 16, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pagedir.h"
#include "index.h"
#include "file.h"
#include "memory.h"
#include "hashtable.h"
#include "counters.h"
#include "word.h"
#include "webpage.h"
#include "filepath.h"


static bool validateArguments(const int argc, char *argv[]);
static hashtable_t *buildInvertedIndex(char *pageDirectory);
static void itemdelete(void *item);
static webpage_t *createPageFromFile(FILE *fp);
static void incrementCount(hashtable_t *index, char *word, int document_id);


int
main(const int argc, char *argv[])
{
  hashtable_t *index = NULL;

  // validate arguments
  if (!validateArguments(argc, argv)) {
    exit(1);
  } 
  // build the hashtable of inverted index
  index = buildInvertedIndex(argv[1]);

  // save the hashtable of inverted index to the indexFilename 
  saveIndexfile(index, argv[2]);

  // delete the hashtable of inverted index after it is saved to a file
  hashtable_delete(index, itemdelete);
}



/***************** validateArguments ******************************/
/* validate command line arguments
 * 
 * Returns true if command line arguments are valid, false otherwise
 */
static bool 
validateArguments(const int argc, char *argv[])
{
  FILE *fp = NULL;
  // verify the number of arguments
  if (argc != 3) {                                            
    printf("usage: ./indexer pageDirectory indexFilename\n");
    return false;
  }

  // verify the first argument pageDirectory
  if (isCrawlerDirectory(argv[1]) == false) { 
    printf("The first argument %s does not exist or is not a Crawler-produced directory\n", argv[1]);
    return false;
  } 

  // verify the second argument indexFilename
  fp = fopen(argv[2], "w"); 
  if (fp == NULL) {
    printf("The second argument indexFileName %s is not writable\n", argv[2]);
    return false;
  } else {
    fclose(fp);
  } 

  return true;
}



/***************** buildInvertedIndex ******************************/
/* Builds the hashtable of inverted index by reading in all the files stored in pageDirectory
 *
 * Returns
 * for example: pageDirectory/document_id
 * 
 * Returns the hashtable of inverted index stored in malloc'd memory
 * the caller is responsible for freeing that space
 * 
 * Assumptions:
 *    1. pageDirectory has files named 1, 2, 3, ..., without gaps
 *    2. The content of files in pageDirectory follow the following format:
 *       the first line is the url
 *       the second line is the depth at which crawler finds it
 *       the rest of the file is the html retrieved from the url
 */
static hashtable_t *buildInvertedIndex(char *pageDirectory)
{ 
  hashtable_t *index = hashtable_new(600);
  int document_id = 1;
  char *filepath = fullFilePath(pageDirectory, document_id);
  FILE *fp = NULL;
  char *word = NULL;
  int pos = 0;

  while ((fp = fopen(filepath, "r"))) {
    // create a webpage from a crawler-produced file
    webpage_t *page = createPageFromFile(fp); 
    fclose(fp);
    // for each word in page, increment the counter for the current document_id
    while ((word = webpage_getNextWord(page, &pos))) {
      // include only words with more than 2 characters into the hashtable index
      if (strlen(word) >= 3) {  
        // normalize word    
        normalizeWord(word);
        // increment the count for the current document_id in the counterset for the word  
        incrementCount(index, word, document_id);
      }
      free(word);
    }
    free(word);
    webpage_delete(page);
    document_id ++;
    pos = 0;
    free(filepath);
    filepath = fullFilePath(pageDirectory, document_id);
  }
  free(filepath);

  return index;
}


/***************** itemdelete ******************************/
/* helper function to pass in the hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item))
 * 
 * Free all memory allocated for a counter set by calling counters_delete(counters_t *ctrs)
 */
static void 
itemdelete(void *item)
{
  counters_t *ctrs = item;
  counters_delete(ctrs);
}



/***************** createPageFromFile ******************************/
/* helper function
 *
 * Construct the webpage from a cralwer-produced file
 * 
 * Returns a webpage stored in malloc'd memory
 * the caller is responsible for freeing that space
 * 
 * If any error when creating a webpage: prints an error message but does not exit the program
 * 
 * Assumptions:
 *    1. file pointer is not NULL
 *    2. The content of files in pageDirectory follows the following format:
 *       the first line is the url
 *       the second line is the depth at which crawler finds it
 *       the rest of the file is the html retrieved from the url
 */
static webpage_t *createPageFromFile(FILE *fp) 
{
  // read in url, depth and html from a crawler-produced file
  char *url = freaduntil(fp, isnewline);
  char *str_depth = freaduntil(fp, isnewline);
  // convert depth as a string form to depth as in int
  int int_depth = sscanf(str_depth, "%d", &int_depth); 
  char *html = freadfilep(fp);
  free(str_depth);

  // create a page from the retrieved url, depth and html
  webpage_t *page = webpage_new(url, int_depth, html);
  if (page == NULL){
    printf("error when creating a webpage for %s\n", url);
  }
  return page;
}




/***************** incrementCount ******************************/
/* helper function
 *
 * increment in the counterset for the word the count for the current document_id
 * 
 * If the counterset for the word exists, increment the count for the document_id
 * If counterset for the word does not exist
 */
static void incrementCount(hashtable_t *index, char *word, int document_id) 
{
  counters_t *ctrs = NULL;
  if ((ctrs = hashtable_find(index, word))) {
    // if counterset for the word exists, increment the count for the document_id
    counters_add(ctrs, document_id);
  } else {
    // if counterset for the word does not exist
    counters_t *ctrs = counters_new();  // create a counterset for the word
    counters_add(ctrs, document_id);    // add the document_id to the counterset and initialize its count value to 1
    hashtable_insert(index, word, ctrs);// insert the counterset for the word to the hashtable of inverted index
  }
}
