/* 
 * crawler.c 
 *
 * The crawler crawls a website and retrieves webpages starting with a specified URL. 
 * It parses the initial webpage, extracts all embedded URLs and retrieves those pages, 
 * and crawls the pages found at those URLs, in a recursive manner, but limits itself 
 * to maxDepth hops from the seed URL and to URLs that are ‘internal’ to the designated CS50 server.
 * 
 * Usage: ./crawler seedURL pageDirectory maxDepth
 * 
 * Args:
 *      seedURL is used as the initial URL 
 *      pageDirectory is the directory in which to write downloaded webpages
 *      maxDepth represents the maximum crawl depth.
 * Exit:
 *      1. incorrect arguments
 *      2. creating a webpage failed when calling webpage_new()
 *      3. inserting the seedURL to the hashtable of seen URLS failed (it shouldn't!)
 *
 * Zitong Wu, Feb 8, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "webpage.h"
#include "pagedir.h"
#include "bag.c"
#include "hashtable.h"


static int validateArguments(const int argc, char *argv[], int max_depth);
void crawler(bag_t *bag, hashtable_t *seen_urls, char* page_directory, int max_depth);
void hs_item_delete(void *item){} // hashtable's itemdelete function


int
main(const int argc, char *argv[])
{
  int max_depth = -1;
  char *page_directory = NULL;
  webpage_t *page = NULL;
  bag_t *pagebag = bag_new();
  hashtable_t *seen_urls = hashtable_new(653); 

  // validate arguments
  max_depth = validateArguments(argc, argv, max_depth);
  page_directory = argv[2];

  // create a webpage from the first argument seedURL
  char *seed_url = malloc(strlen(argv[1]) + 1) ;
  strcpy(seed_url, argv[1]);
  page = webpage_new(seed_url, 0, NULL);
  if (page == NULL) {
    printf("creating a new webpage failed\n");
    exit(2);
  }

  // add the newly created webpage to the bag of pages
  bag_insert(pagebag, page);

  // insert the seedURL to the hashtable of URLS seen
  if (hashtable_insert(seen_urls, seed_url, "") == false) {
    printf("inserting the seedURL to the hashtable of seen URLS failed\n");
    exit(3);
  }

  // while there are more webpages to crawl
  crawler(pagebag, seen_urls, page_directory, max_depth);

  // free all malloc'd memory
  bag_delete(pagebag, webpage_delete);
  hashtable_delete(seen_urls, hs_item_delete);
}


/***************** validateArguments ******************************/
/* validate command line arguments
 * 
 * Returns max_depth read from the command line if all arguments are valid
 * Exit the program otherwise
 */
static int
validateArguments(const int argc, char *argv[], int max_depth)
{
  // verify the number of arguments 
  if (argc != 4) {                                               
    printf("usage: crawler seedURL pageDirectory maxDepth\n");
    exit(1);
  }

  // verify the first argument seedURL
  if (IsInternalURL(argv[1]) == false) {         
    printf("The first argument seedURL is not a valid url or is not internal\n");
    exit(1);
  }

  // verify the second argument pageDirectory
  if (isPageDirectory(argv[2]) == false) {     
    printf("The second argument PageDirectory is not a directory or nor writable\n");
    exit(1);
  } 

  // verify the third argument maxDepth
  if (sscanf(argv[3], "%d", &max_depth) != 1) {  
    printf("The third argument maxDepth must be an integer\n");
    exit(1);
  } else if (max_depth < 0) {
    printf("The third argument maxDepth must be a non-negative integer\n");
    exit(1);
  }

  return max_depth;
}


/***************** crawler ******************************/
/* extracts a page from the bag, until the bag is exhausted
 * for each extracted page:
 *    1. use pageFetcher to retrieve the HTML for the page at that URL and save it to page->html
 *    2. use pageSaver to write the webpage to the pageDirectory with a unique document ID
 *    3. use pageScanner to scan the page for more urls
 *
 * Args:
 *    bag: bag of pages to be crawled
 *    seen_urls: hashtable of seen urls
 *    page_directory: directory where files of webpage information are saved
 *    max_depth: max depth to be crawled
 * 
 * Assumptions: 
 *    bag of pages to be crawled,, hashtable of seen_urls, and pagedirectories are all non-null pointers   
 * 
 */

void crawler(bag_t *bag, hashtable_t *seen_urls, char* page_directory, int max_depth)
{
  int document_id = 1;
  webpage_t *page = NULL;
  bool value;
  
  // extract a webpage item from the bag 
  page = bag_extract(bag);
  while (page != NULL) {
    // use pageFetcher to retrieve the HTML for the page at that URL
    // pause for one second by pageFetcher
    value = pageFetcher(page);
    if (value == false) {
      printf("fetching the webpage at URL %s failed\n", webpage_getURL(page)); // how to log?
    } else {
      logr("Fetched", webpage_getDepth(page), webpage_getURL(page));
      // use pageSaver to write the webpage to the page_directory with the document_id
      pageSaver(page, page_directory, document_id); 
      // if the webpage depth is < maxDepth, explore the webpage to find more URLs
      if (webpage_getDepth(page) < max_depth) {
        // use pageScanner to parse the webpage to extract all its embedded URLs
        logr("Scanning", webpage_getDepth(page), webpage_getURL(page));
        pageScanner(page, seen_urls, bag);
      }
      document_id ++;
    }
    webpage_delete(page);
    page = bag_extract(bag);
  }
}
