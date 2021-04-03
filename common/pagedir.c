/* 
 * pagedir.c 
 *
 * see pagedir.h for more information.
 *
 * Zitong Wu, Feb 8, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"


/**************** logr ****************/
/* see pagedir.h for description */
void logr(const char *word, const int depth, const char *url) 
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url); 
}


/**************** isCrawlerDirectory ****************/
/* see pagedir.h for description */
bool isCrawlerDirectory(char *pageDirectory)
{
  char filename[50] = "";
  strcpy (filename, pageDirectory);
  strcat(filename, "/");
  strcat(filename, ".crawler");

  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    return false;
  } else {
    fclose(fp);
    return true;
  } 
}

/**************** isPageDirectory ****************/
/* see pagedir.h for description */
bool isPageDirectory(char *directory) 
{
  char filename[200] = "";
  strcpy (filename, directory);
  strcat(filename, "/");
  strcat(filename, ".crawler");

  FILE *fp = fopen(filename, "w");
  if (fp == NULL) {
    return false;
  } else {
    fclose(fp);
    return true;
  } 
}

/**************** pageFetcher ****************/
/* see pagedir.h for description */
bool pageFetcher(webpage_t *page) 
{ 
  bool value = webpage_fetch(page);
  return value;
}


/**************** page_saver ****************/
/* see pagedir.h for description */
void pageSaver(webpage_t *page, char *page_directory, const int num_id)
{
  char *filepath = malloc(200 * sizeof(char));
  strcpy (filepath, page_directory);
  char *string_id = malloc(1000 * sizeof(char));
  sprintf(string_id, "%d", num_id);
  strcat(filepath, "/");
  strcat(filepath, string_id);
  free(string_id);

  FILE *fp;
  fp= fopen(filepath, "w"); 
  if(fp == NULL) {
    printf("error when creating and writing a file with id=%d to pageDirectory %s\n", num_id, page_directory);
    exit(4);
  } else {
    fprintf(fp, "%s\n", webpage_getURL(page));
    fprintf(fp, "%d\n", webpage_getDepth(page));
    fprintf(fp, "%s\n", webpage_getHTML(page));
  }
  fclose(fp);
  free(filepath);
}

/**************** pageScanner ****************/
/* see pagedir.h for description */
void pageScanner(webpage_t *page, hashtable_t *hs, bag_t* bag)
{ 
  int pos = 0;
  char *url = NULL;
  webpage_t *new_page = NULL;

  url = webpage_getNextURL(page, &pos);
  while (url != NULL) {
    logr("Found", webpage_getDepth(page), url);
    // normalize url
    if (NormalizeURL(url) && IsInternalURL(url)) {
      // if url is internal, insert that URL into the hashtable of seen urls
      if (hashtable_insert(hs, url, "") == true) {
        // if the url is successfully inserted into the hashtable of seen urls
        logr("Added", webpage_getDepth(page), url);  
        // make a new webpage for that URL, at depth+1
        char *url_copy = malloc(strlen(url) + 1); 
        strcpy(url_copy, url);
        new_page = webpage_new(url_copy, webpage_getDepth(page) + 1, NULL);
        if (new_page == NULL) {
          printf("creating a new webpage failed\n");
          exit(2);
        }
        // add the new webpage to the bag of webpages to be crawled
        bag_insert(bag, new_page);
      } else {
        // if url already seen, print logging information
        logr("IgnDupl", webpage_getDepth(page), url);
      }
    // if url is not internal, print logging information
    } else {
      logr("IgnExtrn", webpage_getDepth(page), url);
    }
    free(url);
    url = webpage_getNextURL(page, &pos);
  }
  free(url);

}
