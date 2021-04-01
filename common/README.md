## Zitong Wu

<br >

### Usage
In the common directory,   
`make` or `make all`: builds and compiles all these utility functions (`pagedir` `index`, `word` and `filepath`) into a library common.a  
`make clean`: clean up the common directory

### pagedir

Utility functions shared by the crawler, indexer, and querier:

 *    logr: prints logging information as pages are found
 *    is_page_directory: checks if a directory  exists and is writable
 *    page_fetcher: fetches a page from a page->URL and store it into page->html
 *    page_saver: outputs a page to the the appropriate file
 *    page_scanner: extracts URLs from a page and processes each one


### index
Utility functions shared by the crawler, indexer, and querier:  

* loadIndexfile: load the index from a file into an hashtable of
inverted-index mapping from word to pairs of (document_id count)
*  saveIndexfile: create a new file and write a hashtable of inverted index to that file in the format described below

 *     The content of the index file follows the following format:
 *     one line per word, one word per line
 *     each line provides the word and one or more (docID, count) pairs, in the format
 *     word docID count [docID count]... 
 *     where word is a string of lower-case letters
 *     where docID is a positive non-zero integer
 *     where count is a positive non-zero integer
 *     where the word and integers are separated by spaces.

### word
Utility function shared by the crawler, indexer, and querier:
   
 *    normalizeWord: returns a word that converts all the letters of the input word to lower-case. The returned word points to malloc'd space. The caller is responsible for freeing that space.

 
### filepath (I included this utility function for convenience)
Utility function shared by the crawler, indexer, and querier:

* `fullFilePath`: construct the full file path with a given pageDirectory and a document_id
* `isnewline`: Stop function for use with freaduntil()
