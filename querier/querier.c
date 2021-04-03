/* 
 * querier.c
 * 
 * Reads the index file produced by the Indexer, 
 * and page files produced by the Crawler, 
 * and answers search queries submitted via stdin. 
 * 
 * Usage: ./querier pageDirectory indexFilename
 * 
 * Args:
 *    pageDirectory is the directory in which files produced by the crawler are saved
 *    indexFilename is the filename to save the index
 * 
 * Exit status:
 *    0: When EOF is reached on stdin.
 *    1: incorrect arguments
 *
 * Assumptions:
 *    1. pageDirectory has files named 1, 2, 3, ..., without gaps
 *    2. The content of files in pageDirectory follows the following format:
 *      the first line is the url
 *      the second line is the depth at which crawler finds it
 *      the rest of the file is the html retrieved from the url
 *    3. The content of the file named by indexFilename follows the following format:
 *      one word per line
 *      word docID count [docID count]... 
 *      where word is a string of lower-case letters
 *      where docID is a positive non-zero integer
 *      where count is a positive non-zero integer
 *      where the word and integers are separated by spaces.
 *
 * Zitong Wu, Feb 21, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "pagedir.h"
#include "file.h"
#include "word.h"
#include "counters.h"
#include "hashtable.h"
#include "index.h"
#include "file.h"
#include "memory.h"
#include <unistd.h>
#include "filepath.h"
#include "counters_helper.h"

/**************** file-local global variables ****************/
int fileno(FILE *stream);

static bool validateArguments(const int argc, char *argv[]);
static bool areCharsLegal(char *string);
static char **parseQuery(char *string);
static bool isQueryLegal(char **array);
static void printQuery(char **array);
static void itemdelete(void *item);
static counters_t *query(hashtable_t *hs, char **array);
static counters_t *andsequence(hashtable_t *hs, char **array, int i);
static void printResult(struct counter **array, int num_items, char *pageDirectory);
static void prompt(void);


int
main(const int argc, char *argv[])
{
  hashtable_t *index = NULL;
  char *input = malloc(100*sizeof(char));
  counters_t *result = NULL;
  struct counter **sorted_result = NULL;
  char *pageDirectory = NULL;
  int num_items = 0;


  // validate arguments
  if (!validateArguments(argc, argv)) {
    exit(1);
  } 
  pageDirectory = argv[1];
  // load index
  index = loadIndexfile(argv[2]);

  prompt();
  while (fgets(input, 100, stdin) != NULL) {
    // check if input contains only spaces or letters
    if (areCharsLegal(input)) {
      // parse the input into an array of words
      char **parsed = parseQuery(input);
      
      // if input is not empty
      if (parsed[0] != NULL) {
        // print cleaned, parsed query
        printQuery(parsed);
        if (isQueryLegal(parsed)){
          // if query is legal
          result = query(index, parsed);                        // find result
          num_items = numItems(result);                       
          sorted_result = sortCounters(result, num_items);      // sort result
          printResult(sorted_result, num_items, pageDirectory); // print result
          counters_delete(result);
          for (int i = 0; i < num_items; i ++) {                // free sorted_result
            free(sorted_result[i]);
          }
          free(sorted_result);
        } else {
          printf("query contains operators at incorrect positions\n");
        }
      } else {
      }
      free(parsed);                    // free parsed
    } 
    prompt();
  }
  hashtable_delete(index, itemdelete); // free index
  free(input);                         // free input

  exit(0);
}

/***************** validateArguments ******************************/
/* validate command line arguments
 * 
 * Returns true if command line arguments are valid, false otherwise
 */
static bool 
validateArguments(const int argc, char *argv[])
{
  //verify the number of arguments
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
  FILE *fp = NULL;
  fp = fopen(argv[2], "r"); 
  if (fp == NULL) {
    printf("The second argument indexFileName %s does not exist or is not readable\n", argv[2]);
    return false;
  } else {
    fclose(fp);
  } 

  return true;
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

/***************** printQuery ******************************/
/* Print each word in a query (each item in an array), separated by space 
 * 
 * Note: this function should be called on a cleaned, parsed query
 */
static void printQuery(char **array)
{ 
  if (array[0] != NULL) {
    printf("Query:");
    for (int i = 0; array[i] != NULL; i++){
      printf(" %s", array[i]);
    }
    printf("\n");
  }
}

/***************** printResult ******************************/
/* Print the query result in the following way:
 *    print one matched document per line
 *    on each line: list the score, document ID and URL
 *    example: score   6 doc   2: http://cs50tse.cs.dartmouth.edu/tse/wikipedia/
 * 
 * Note: this function should be called on a cleaned, parsed query
 */

static void printResult(struct counter **array, int num_items, char *pageDirectory){
  char *url = NULL;
  char *filepath = NULL;
  FILE *fp = NULL;

  if (num_items == 0) {
    printf("No documents match.\n");
  } else {
    printf("Matches %d documents (ranked): \n", num_items);
    for (int i = 0; i < num_items; i++) {
      filepath = fullFilePath(pageDirectory, array[i]->doc_id);
      if ((fp = fopen(filepath, "r"))) {
        url = freaduntil(fp, isnewline);
        printf("score%4d doc%4d: %s\n", array[i]->count,  array[i]->doc_id, url);
        free(url);
        fclose(fp);
      }
      free(filepath);
  }
  }
  printf("-----------------------------------------------------\n");
}


/***************** andsequence ******************************/
/* Return set of documents that satisfy an andsequence
 *    by looping over words in an andsequence and 
 *    accumulating an answer (successively computing intersections) as you go
 * 
 * Note: andsequence is a sequence of words connected by "and" operators, 
 *    for example: "computer and telephone and house";
 *    words that are conncected by spaces are also an andsequence,
 *    for example: "white lily flower";
 *    the result of an andsequence is the intersection of multiple counter sets
 * 
 * The caller is responsible for freeing the malloc'd space of the returned item
 */
static counters_t *andsequence(hashtable_t *hs, char **array, int i){
  counters_t *temp = hashtable_find(hs, array[i]);
  counters_t *temp_holder = NULL;
  counters_t *ctrs = NULL;
  int c = 0;
  for (int j = i; (array[j] != NULL) && (strcmp(array[j], "or")); j++) {
    if (strcmp(array[j], "and")) {
      ctrs = hashtable_find(hs, array[j]);
      temp_holder = temp;
      temp = countersIntersection(temp, ctrs);
      if (c != 0) {
        counters_delete(temp_holder);
      }
      c ++;
    }
  }
  return temp;
}

/***************** query ******************************/
/* Return set of documents that satisfy a query
 *    by looping over calls to the andsequence() function for each andsequence
 *    accumulating an answer (successively computing intersections) as you go
 * 
 * Note: query effectively compute an orsequence
 *    the result of an orsequence is the union of multiple counter sets
 * 
 * The caller is responsible for freeing the malloc'd space of the returned item
 */
static counters_t *query(hashtable_t *hs, char **array){
  counters_t *result = counters_new();
  counters_t *temp = andsequence(hs, array, 0);
  for (int i = 1; array[i] != NULL; i++) {
    if (!strcmp(array[i], "or")) {
      countersUnion(result, temp);
      counters_delete(temp);
      temp = andsequence(hs, array, i+1);
    }
  }
  countersUnion(result, temp);
  counters_delete(temp);
  return result;
}



/***************** areCharsLegal ******************************/
/* check if all characters in a string are either letters or spaces
 *
 * Returns true if all characters in a string are either letters or spaces
 * Returns false if some characters in a string are not letters nor spaces
 */ 
static bool areCharsLegal(char *string){
  for(char *c = string; *c != '\n' ; c++) {
    if ( (!isalpha(*c)) && (!isspace(*c))) {
      printf("query should only contain spaces and letters\n");
      return false;
    }
  }
  return true;
}

/***************** parseQuery ******************************/
/* Returns a clean and parsed query as an array of char pointers
 *
 * Clean and parsed query always starts with a word, and may be
 * followed by more words separacted by a single space, for example:
 *    computer
 *    computer science and design
 * 
 * Assumption: the input string contains only spaces or letters
 * 
 * Note: if the query is blank line (i.e., empty or nothing but spaces),
 * Returns an array of NULL pointers
 * 
 * The caller is responsible for freeing the malloc'd space of the returned item
 */ 
static char **parseQuery(char *string){
  char *start = string;
  char *end = NULL;
  char **array = calloc(100, sizeof(char*));
  int i = 0;
  char *c = start;
  while (*c != '\0') {
    // find the start of the word
    if (isalpha(*c)) {
      start = c;
      end = c++;
      bool value = true;
      // find the end of the word
      while (value) {
        if (isspace(*end)) {
          *end = '\0';
          normalizeWord(start);
          array[i] = start;
          i++;
          start = end++;
          value = false;
        }       
        else {
          end ++;
        }
      }
      c = end++;
    } else {
      c++;
    }
  }
  return array;
}

/***************** isQueryLegal ******************************/
/* Check if a parsed query is legal, that is:
 *    neither the first or last word may be an operator,
 *    and two operators may not be adjacent
 * 
 * Returns true if a parsed query is legal
 * Returns false if a parsed query is not legal
 * 
 * Assumption: the input is parsed query as an array of char pointers
 */ 
static bool isQueryLegal(char **array){
  int i = 0;

  // check if the first word in the array is an operator
  if (!strcmp(array[0], "and") || !strcmp(array[0], "or")) {
    return false;
  }

  // check if two adjacent words are both operators
  for (i = 1 ; array[i] != NULL; i++){
    bool value1 = (!strcmp(array[i], "and") || !strcmp(array[i], "or"));
    bool value2 = (!strcmp(array[i - 1], "and") || !strcmp(array[i - 1], "or"));
    if (value1 && value2) {
      return false;
    }
  }
  // check if the last word in the array is an operator
  if (!strcmp(array[i - 1], "and") || !strcmp(array[i - 1], "or")) {
    return false;
  }

  return true;
}


/****************** prompt ******************************
 * Print a prompt "Query?" for an interactive user 
 * and not print a prompt when the stdin is not a keyboard
 * 
 * Note: The fileno() function is provided by stdio,  
 * but for some reason not declared by stdio.h, so declare here.  
 */ 
static void
prompt(void)
{
  // print a prompt iff stdin is a tty (terminal) 
  if (isatty(fileno(stdin))) { 
    printf("Query? "); 
  }
}
