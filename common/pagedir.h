/* 
 * pagedir.h 
 *
 * Utility functions shared by the crawler, indexer, and querier:
 *    logr: prints logging information as pages are found
 *    isCrawlerDirectory: checks if a directory is produced by crawler
 *    isPageDirectory: checks if a directory  exists and is writable
 *    pageFetcher: fetches a page from a page->URL and store it into page->html
 *    pageSaver: outputs a page to the the appropriate file
 *    pageScanner: extracts URLs from a page and processes each one
 * 
 * Exit:
 *      2. creating a new webpage failed when calling webpage_new()
 *      4. error when creating or writing to a file with the page's information to pageDirectory/id
 * 
 * Zitong Wu, Feb 8, 2021
 */

#ifndef __PAGEDIR_H__
#define __PAGEDIR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"



/***************** logr ******************************/
/* prints logging information for the newly found url
 *
 * Assumptions:
 *     depth: the depth of the page currently being crawled, not that of the newly found page
 *     word: 
 *          Found: when a valid url is found, whether it is internal or not
 *          Added: if it is the first time that the url is seen and thus newly added to the hashtable of seen urls
 *          IgnDupl: if the url is seen before
 *          IgnExtrn: if the url is not internal
 * 
 * Note: this function also creates pageDirectory/.crawler file, which
 * will later help the indexer confirm the pageDirecotry is indeed a Crawler-produced directory
 */
void logr(const char *word, const int depth, const char *url);


/***************** isCrawlerDirectory ******************************/
/* checks if a directory is a crawler-produced directory 
 * by checking if the directory contains the file .crawler
 *
 * Returns:
 *     True: if a directory is a crawler produced directory 
 *     False: if a directory is not a crawler produced directory 
 */
bool isCrawlerDirectory(char *dir);


/***************** isPageDirectory ******************************/
/* checks if a directory exists and is writable
 *
 * Returns:
 *     True: if a directory exists and is writable
 *     False: if a directory does not exist or is not writable
 * 
 * Note: this function also creates pageDirectory/.crawler file, which
 * will later help the indexer confirm the pageDirecotry is indeed a Crawler-produced directory
 */


bool isPageDirectory(char *directory);

/***************** pageFetcher ******************************/
/* outputs a page to the the appropriate file
 * Returns true if the fetch was successful; otherwise, false. 
 * If the fetch succeeded, page->html will contain the content retrieved. 
 * Parameter 'page' should have been allocated by the caller, 
 * but the page->html pointer is expected to be NULL. 
 * If this function is successful, a new, null-terminated character
 * buffer will be allocated as page->html. The caller must later free this
 * memory, typically by calling webpage_delete().
 *
 * Assumptions:
 *     1. page has been allocated by caller
 *     2. page->url contains the url to fetch
 *     3. page->html is NULL at call time
 * 
 *  * Returns:
 *     True: success; caller must later free html via webpage_delete(page).
 *     False: some error fetching page.
 * 
 * Limitations:
 *   * can only handle http (not https or other schemes)
 *   * can only handle URLs of form http://host[:port][/pathname]
 *   * cannot handle redirects (HTTP 301 or 302 response codes)
 */
bool pageFetcher(webpage_t *page);


/***************** pageSaver ******************************/
/* create a file as pageDirectory/id and save it to pageDirectory
 * the file writes the following information:
 * 1. the first line of the file is the URL
 * 2. the second line of the file is the depth
 * 3. the rest of the file is the page content (the HTML, unchanged)
 *
 * Assumptions:
 *     1. webpage is a non-null pointer
 *     2. page_directory exists and is writable
 *     3. id is an integer number equal to or greater than 1
 */
void pageSaver(webpage_t *page, char *page_directory, const int num_id);



/***************** pageScanner ******************************/
/* extracts all URLs from a page and processes each URL in the following way
 * 1. normalize the URL
 * 2. if that URL is not internal, ignore it
 * 3. try to insert that URL into the hashtable of URLs seen
 *    3.1 if it was already in the table, do nothing
 *    3.2 if it was added to the table,
 *        3.2.1 make a new webpage for that URL, at depth+1
 *        3.2.2 add the new webpage to the bag of webpages to be crawled
 * 
 * Assumptions:
 *     1. webpage is a non-null pointer
 *     2. hashtable of seen urls is a non_null pointer
 *     3. bag of pages to be crawled is a non-null pointer
 */
void pageScanner(webpage_t *page, hashtable_t *hs, bag_t *bag);
 
#endif 
