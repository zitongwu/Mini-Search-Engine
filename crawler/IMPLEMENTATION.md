# Crawler
<br > 

## IMPLEMENTATION SPEC
### Zitong Wu
<br > 
<br >

### Functions
In the crawler folder, crawler.c has 2 functions: main, crawler 

 * `main(const int argc, char *argv[])`:   
     * check the number of arguments (should be 3)
     * IsInternalURL to check if the first argument seedURL is internal
     * use is_page_directory to check if the second argument pageDirectory exists and is writable
     * check if the third argument is a non-negative integer
     * use webpage_new() to create a webpage from the first argument seedURL
     * insert the newly created webpage to the bag of pages to be crawled
     * insert the seedURL to the hashtable of URLS seen
     * use crawl to iteratively find new webpages, insert new webpages to the bag of pages, and save the page information to a file in pagedirectory
     * free bag of pages and hashtable of seen urls
     
 * `void crawler(bag_t *bag, hashtable_t *seen_urls, char* page_directory, int max_depth)`: 
 	 * extract all webpages from the bag, until the bag is exhausted 
 	 * for each extracted webpage: 1) use page\_fetcher to retrieve the HTML for the page at that URL, pause for one second; 2) if the HTML is successfully fetched from the page's url, use page\_saver to save the page information to a file in pageDirectory with the document_id; 3)if the depth of the page currently being crawled is less than max_depth, scan the page for more urls
 	 * increment document_id by 1
 	 
### Helper Functions:
In the common folder, pagedir.c has 5 functions, see pagedir.h for detailed information on each function:  
 
 *    logr: prints logging information as pages are found
 *    is\_page_directory: checks if a directory  exists and is writable
 *    page_fetcher: fetches a page from a page->URL and store it into page->html
 *    page_saver: outputs a page to the the appropriate file
 *    page_scanner: extracts URLs from a page and processes each one

### Data structures:

* typedef struct webpage webpage_t contains: `char *url`, `char *html`, `int depth`                    
* bag of page structures
* hashtable of URLs: url as the key, "" as the item stored in the key
* set of URLs (indirectly used by hashtable)

### Exit Status:
 * 1: incorrect arguments
 * 2: creating a webpage failed when calling webpage_new()
 * 3: inserting the seedURL to the hashtable of seen URLS failed (it shouldn't!)
 * 4: error when creating or writing the page's information to pageDirectory/id

 
### Assumptions:
 
 * The pageDirectory does not contain any files whose name is an integer (i.e., 1, 2, ...)
 * Internal URL: Only those beginning "http://cs50tse.cs.dartmouth.edu/tse/"
 * To be polite, our crawler purposely slows its behavior by introducing a delay, sleeping for one second between fetches
 