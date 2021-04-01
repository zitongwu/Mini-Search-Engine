# Querier
<br > 

## IMPLEMENTATION SPEC
### Zitong Wu
<br > 
<br > 


### Detailed pseudo code 
**querier.c has 4 main functions:** `main`, `parseQuery`, `query`, `andsequence`

 * `main(const int argc, char *argv[])`:   
     * check the number of arguments (should be 3)
     * use `isCrawlerDirectory` from `pagedir.c` located in the common folder to check if the first argument pageDirectory is a crawler-produced directory by checking if pageDirectory contains .crawler file
     * check if the second argument indexFilename is readble, whether it already exists or is newly created by calling `fopen(argv[2], "w")`
     * use `loadInvertedIndex` to load the hashtable of inverted index from `indexFilename`
     * ask for user input (query) until EOF is reached on stdin:
    
     	*  checks if all characters in a query are either letters or spaces, using `areCharsLegal` 
		* if true, parse the query into an array of char pointers; if false, ask for another user input, using `parseQuery`
		* print the parse query to stdout
		* checks if the parsed query is legal, that is containing no operators at incorrect places, using `isQueryLegal`
		* find page files that match with the query, by calling `query`
		* sort the query result, using `sortCounters`
		* print the query result in the format as decribed in the output section, using `printResult`
	4.5 free all malloc'd spaces
     * free the memory allocated for the input and hashtable of inverted Index
  
 * `static char **parseQuery(char *string)`: scan through each character in the string, upon encountering the first letter, mark it as the start of the first word, upon encounterting the first space after the first letter, mark it as the end of the first word and set it to '\0', and do this repeatedly until reaching the end of the string
   
 * `static counters_t *query(hashtable_t *hs, char **array)`:
  
	* create a new counter set `result`
	* create and point temp to the andsequence called on the first word in the array
	* starting from the second word in the array, whenever the word is "or":
	
		* perform `countersUnion` on result and temp
		* free the space allocated to temp
		* point temp to a new andsequence called on the next word in the array
	* perform `countersUnion` on result and temp for a final round
	* free the space allocated to temp
	* return result

	
 * `static counters_t *andsequence(hashtable_t *hs, char **array, int i)`:
  
	* point temp to `hashtable_find(hs, array[i])`
	* create a `temp_holder` pointing to NULL
	* create a counter set `ctrs` pointing to NULL
	* starting from array[i] in the array, as long as it is not NULL or an "or operator":
	
		* point `ctrs` to `hashtable_find(hs, array[i])`
		* point `temp_holder` to `temp`
		* point `temp` to the `countersIntersection(temp, ctrs)`
		* free the space that `temp_holder` points to (except the first time)
	* return temp


**querier.c has the following helper functions:**

* `static bool areCharsLegal(char *string)`:
	* loop through each character in the string, whenenver a character that is neither a character nor a space is encountered, return false; else return true;
	
* `static bool isQueryLegal(char **array)`:
	* check if the first word in the array is an operator
	* check if two adjacent words are both operators
		*starting from i=1, check if array[i] and array[i-1] are both operators 
	* check if the last word in the array is an operator

* `static void printQuery(char **array)`: print each word in a array (each item in an array), separated by space 

* `static void itemdelete(void *item)`: casts item into a counter set and performs `counters_delete()`

* `static void printResult(struct counter **array, int num_items, char *pageDirectory)`: 
	* if num_items is 0, prints "No documents match.\"
	* else prints the number of matched documents
	* for each matched document, prints the ranking score, document_id and url

* `static void prompt(void)`: if `isatty(fileno(stdin)`, then print the prompt


**counters_helper.c has the following helper functions, all related to operations on counter set or counter object (struct type defined in counters_helper.h, containing doc_id and url):**
	
* `counters_t *countersIntersection(counters_t *ctrsA, counters_t *ctrsB)`:

	* create a new counter set `result`
	* create a twocountersets (struct defined in this file that contains two counter sets, see datastructure section below) object containing `result` and `ctrsB`
	* call `counters_iterate` on`ctrsA` with the pointer to the twocountersets as the arg and `countersIntersectionHelper` as the itemfunc 
	
* `void countersIntersectionHelper(void *arg, const int current, const int count)`:
	* cast the arg as a twocountersets object(struct defined in this file that contains two counter sets, see datastructure section below)
	* define counter set `result` as the first counterset in the twocountersets; define counter set `ctrsB` as the second counterset in the twocountersets
	* define `int countA` as `count`
	* get ` int countB` of the `key` in `ctrsB`
	* Take the mininum of `countA` and `countB` as the count of their intersection `result`
	
* `void countersUnion(counters_t *ctrsA, counters_t *ctrsB)`: call `counters_iterate` on `ctrsB` with the `ctrsA` as the arg and `countersUnionHelper` as the itemfunc

* `void countersUnionHelper(void *arg, const int key, const int count)`:
	
	* cast arg as counter set object `ctrsA`
	* define `int countB` as `count`
	* get `int countA` of the `key` in `ctrsA`
	* if `countA` is zero (key does not exist in `ctrsA`), set `countB` as the value for the key in `ctrsA`
	* else add `countB` to `countA` for the key value in `ctrs`
	
* `struct counter **sortCounters(counters_t *ctrs, int num_items)`, which returns a sorted array of counter objects (struct type defined in counters_helper.h

	* create an array of counter objects (struct type defined in counters_helper.h, containing doc_id and url) using malloc
	* call `insertCounterIntoArray` to insert each counter in the counter set ctrs into the array
	* call `insertionSort` to sort the array in place
	
* `void insertCounterIntoArray(void *arg, const int key, const int count)`: helper function to pass in `counters_iterate` 

	* cast arg as an array of counter objects (struct type defined in counters_helper.h, containing doc_id and url)
	* create a counter object using malloc
	* set the doc_id and key of the counter object to key and count
	* put the counter object to the first position in the array that is null (unfilled)
	 
* `void insertionSort(struct counter **array, int num_items)`:
slight modification on https://www.geeksforgeeks.org/insertion-sort/
* `int min(int a, int b)`: returns the minimum of the two integers
* `void itemCount(void *arg, const int current, const int count)`: helper function to pass in `counters_iterate`

	* cast arg as an integer pointer
	* increment the integer pointer for each non-null counter in the counter set
* `int numItems(counters_t *ctrs)`: call `itemCount` on `counters_iterate`

### Data structures:
1. In `counters_helper.c`, we define a global struct:

```
struct counter {
  int doc_id; 
  int count;
};
```
and a local struct:

```
struct twocountersets {
  counters_t *ctrs1; 
  counters_t *ctrs2;
};
```

2. Other data structures used by querier:

  * ***hashtable*** of inverted index mapping from words to (documentID, count) pairs
  
```
  typedef struct hashtable {
  	set_t **array;
  	int length;
 } hashtable_t; 
```
 
  * ***counters*** for each word containing (document_id count) pairs
  * 
```
  typedef struct counters {
  struct counternode *head;
  } counters_t;
```

  * ***webpage*** contains the URL, the depth at which it was fetched, and the html of a webpage
  * 
```
 typedef struct webpage {
  char *url;
  char *html;
  size_t html_len;
  int depth;
  } webpage_t;
```
 
  * ***set*** of countersets (indirectly used by hashtable)
  * 
```
  typedef struct setnode {
  void *item;
  char *key;
  struct setnode *next;
  } setnode_t;
```

### Exit Status:
 * 0: when EOF is reached on stdin
 * 1: incorrect arguments

 
### Assumptions:
 
1. pageDirectory has files named 1, 2, 3, ..., without gaps
2. The content of files in pageDirectory follows the following format:
 *      the first line is the url
 *      the second line is the depth at which crawler finds it
 *      the rest of the file is the html retrieved from the url
3. The content of the file named by indexFilename follows the following format:
 *      one word per line
 *      word docID count [docID count]... 
 *      where word is a string of lower-case letters
 *      where docID is a positive non-zero integer
 *      where count is a positive non-zero integer
 *      where the word and integers are separated by spaces.
4. **the `indexFilename` must be the index file for the page files saved in `pageDirectory`**
 
### Memory:
Upon exiting from the querier program, all malloc'd memory should be freed.
