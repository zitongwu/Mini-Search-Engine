# Indexer
<br > 

## IMPLEMENTATION SPEC
### Zitong Wu
<br >

<br > 

### Detailed pseudo code:
**In the crawler folder, indexer.c has 2 main functions:** `main`, `buildInvertedIndex`

 * `main(const int argc, char *argv[])`:   
     * check the number of arguments (should be 3)
     * use `isCrawlerDirectory` from `pagedir.c` located in the common folder to check if the first argument pageDirectory is a crawler-produced directory by checking if pageDirectory contains .crawler file
     * check if the second argument indexFilename is writable, whether it already exists or new created by calling `fopen(argv[2], "w")`
     * use `buildInvertedIndex` to build the hashtable of inverted index
     * use `saveIndexfile` from `index.c` located in the common directory to save the hashtable of inverted index to indexFilename
     * free the memory allocated for the hashtable of inverted Index
     
 * `hashtable_t *buildInvertedIndex(char *pageDirectory)`:
  
	* set `document_id = 1`
	* use `fullfilepath` function to create the full `filepath` for the first document, in the form of pageDirectory/1
	* while the file `filepath `is readable
   
   		* use `createPageFromFile` function to extract the url, depth and html from the file and construct a webpage from the retrieved url, depth and html 
		* use `webpage_getNextWord` to extract a word from the webpage 
		* while there is still word that can be extracted	
			* if word length is greater than 2
		   * use `normalizeWord` function from `word.c` located in the common folder to create a normalized word
		   * use `incrementcount` function to increment the count for the current file id in the counterset for the word
		* increment document\_id and use create `fullfilepath` function to create a new `filepath` for the next document_id
	* return the built hashtable of inverted index
		 
**In the indexer folder, indexer.c has the following helper functions:**

* `static void itemdelete(void *item)`: item delete function to pass in the `hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item))`

	* cast item into `counters_t`
	* use `counters_delete` to delete the item

* `static char *fullFilePath(char *pageDirectory, int document_id)`: construct the full file path from a given pageDirectory and a document_id in the form of pageDirectory/document_id

* `static webpage_t *createPageFromFile(FILE *fp)`: construct the webpage from a cralwer-produced file
	* use `freaduntil()` and `isnewline()` to get the url from the first line of the file, the depth from the second line of the file, and use `freadfilep` to get the remainder of the file as html
	* use `webpage_new` to create a new webpage from the retrieved url, depth and html

* `static int isnewline(int c)`: stop function for use with `freaduntil()`

* `static void incrementCount(hashtable_t *index, char *word, int document_id)`: increment the count for the current document_id in the counterset for the word 

	* search for the word as a key in the hashtable of inverted index
	* if word exists in the hashtable of inverted index, increment the count for the current file id in the word's counterset
	* if word does not exist in the hashtable of inverted index, create a new counterset for the word, add the file id to the counterset, initialize its count value to 1, and insert the word's counterset to the hashtable of inverted index

**In the common folder, there are two utility functions:**
In the common folder, pagedir.c has 5 functions, see pagedir.h for detailed information on each function:  
In `index.c`:

* `loadIndexfile` recreates from an index file a hashtable of inverted index mapping from each word to pairs of (document_id, count)

	* open the file to read
	* while `fscanf` can find a word	
		* create a new counter set for the word
		* while `fscanf` can find a pair of document_id and count
			* set the count value for the document_id in the word's counterset
		* insert the counter set for the word to the hashtable of inverted index

* `saveIndexfile` creates a new file and writes the hashtable of inverted index to that file
	* call `fopen` to create and open a file to write in
	* call `hashtable_iterate` and pass `keyItemPrint` as one of the arguments to print each word and its counterset 

* `keyItemPrint`: helper function to pass into `hashtable_iterate` in `saveIndexfile`. Write a key word and its counterset onto a line of a file. For example:  
word docID1 count1 docID2 count2

* `itemPrint`: helper function to pass into `keyItemPrint`.  Write one counter (document_id count) onto a line of a file

In `word.c`: 

* normalizeWord: returns a word that converts all letters in the original word to lowercase. 
*
	* Malloc memory for wordcopy
	* copy the input word into wordcopy
	* convert each character in wordcopy to lowercase iteratively
	* return wordcopy

In `pagedir.c`

* `isCrawlerDirectory`: check if pageDirectory is a crawler-produced directory. Returns true if pageDirectory/.crawler can be opened and is readable

### Data structures:

  1. ***hashtable*** of inverted index mapping from words to (documentID, count) pairs
```
  typedef struct hashtable {
  	set_t **array;
  	int length;
 } hashtable_t; 
```
 
  2. ***counters*** for each word containing (document_id count) pairs
```
  typedef struct counters {
  struct counternode *head;
  } counters_t;
```

  3. ***webpage*** contains the URL, the depth at which it was fetched, and the html of a webpage
```
 typedef struct webpage {
  char *url;
  char *html;
  size_t html_len;
  int depth;
  } webpage_t;
```
 
  4. ***set*** of countersets (indirectly used by hashtable)
```
  typedef struct setnode {
  void *item;
  char *key;
  struct setnode *next;
  } setnode_t;
```

### Exit Status:
 * 1: incorrect arguments
 * 2: file pointer is NULL when calling `saveIndexfile`

 
### Assumptions:
 
 * pageDirectory has files named 1, 2, 3, ..., without gaps
 * The content of files in pageDirectory follows the following format:
 	*       the first line is the url
 	*       the second line is the depth at which crawler finds it
 	*       the rest of the file is the html retrieved from the url
 
### Memory Memory:
Upon exiting from the indexer program, all allocated memory should be freed.