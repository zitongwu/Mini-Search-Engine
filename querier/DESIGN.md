# Querier
<br > 

## REQUIREMENTS SPEC
### Zitong Wu
<br > 
<br > 

## DESIGN

### User Interface
The indexer's only interface with the user is on the command-line; it must always have two arguments.

```bash
$ ./querier pageDirectory indexFilename
```

- `pageDirectory` is the directory in which files produced by the crawler are saved
- `indexFilename` is the filename where the index is saved

For example, the crawler produced page files stored in `data/depth2`, and the index file for these page files is `indexFile`, we would use this command-line:

``` bash
$ ./querier data/depth2 indexFile
```

Note: the `indexFilename` must be the index file for the page files saved in `pageDirectory`

### Inputs and outputs

*Input:* the inputs are command-line parameters and the `pageDirectory` where the crawler-produced page files are saved by the first command-line parameter and the `indexFilename` where the index is saved by the second. See the User Interface above.

*Output:* querier reads the index file produced by the Indexer, and page files produced by the Crawler, and answers search queries submitted via stdin in the following format:

* on the first line prints the number of page files that match the query
* prints one matched page file per line, in decreasing rank order
* on each line, list the ranking score, document ID and URL

### Functional decomposition into modules

* We anticipate the following modules or functions for `querier.c`:

 1. *main*, which parses arguments and calls the following functions to perform the query
 2. *preprocessQeury*: this module is only conceptual and will actually be written as three parts
 	* *areCharsLegal*, first checks if all characters in a query are either letters or spaces 
 	* *parseQuery*, then turns a query into a clean and parsed query as an array of char pointers (this function is only called when it is checked that all characters in the query are legal, see *areCharsLegal*)
 	* *isQueryLegal*, finally checks if a parsed query is legal, that is: neither the first or last word may be an operator, and two operators may not be adjacent
 	* *printQuery*, which prints to the user each word in a query (each item in an array), separated by space
 3. *query*, which returns set of documents that satisfy a query by looping over calls to the andsequence() function for each andsequence
accumulating an answer (successively computing intersections) as you go
 4. *andsequence*, which returns set of documents that satisfy an andsequencevby looping over words in an andsequence and accumulating an answer (successively computing intersections) as you go
 5. *printResult*, which prints the query result in the way described in the output section


* I also put some helper functions in `counters_helper.h` and `counters_helper.c`, all related to counter and counter set operations:  

	* *countersIntersection*, which returns the intersection of two counter sets
	* *countersIntersectionHelper*, helper function for *countersIntersection*
	* *countersUnion*, which merges the second counter set into the first counter set
	* *countersUnionHelper*, helper function for *countersUnion*
	* *sortCounters*, which returns a sorted array of counter objects (type defined at the top of this file)
	* *insertCounterIntoArray*, helper function for *sortCounters*, which inserts a counter object (type defined at the top of this file) into an array
	* *insertionSort*, which sorts an array of counter objects (type defined at the top of this file) in decreasing order

And some helper modules that provide data structures:

  1. ***hashtable*** of inverted index mapping from words to (documentID, count) pairs
  2. ***counters*** for each word containing (document_id count) pairs
  3. ***webpage*** which contains the URL, the depth at which it was fetched, and the html of a webpage
  4. ***set*** of countersets (indirectly used by hashtable)



### Pseudo code for logic/algorithmic flow
The querier will run as follows:

1. start from a command line as shown in the User Interface
2. parse the command line, validate arguments
3. load the hashtable of inverted index from `indexFilename`, using `loadIndexfile` from `index.c`
4. ask for user input (query) until EOF is reached on stdin
 
	for each query:
	4.1 preprocess query
	
		4.1.1 checks if all characters in a query are either letters or spaces, using `areCharsLegal` 
		4.1.2 if true, parse the query into an array of char pointers; if false, ask for another user input, using `parseQuery`
		4.1.3 checks if the parsed query is legal, that is containing no operators at incorrect places, using `isQueryLegal`
	4.2 find page files that match with the query, by calling `query`
	4.3	 sort the query result, using `sortCounters`
	4.4 print the query result in the format as decribed in the output section, using `printResult`
	4.5 free all malloc'd spaces
4. delete the hashtable of inverted index and exit

### Dataflow through modules
 1. *main* parses parameters and passes the `pageDirectory` to `printResult` (when urls needs to be retrieved from matched document ids) and `indexFilename` to `loadIndexfile()` to recreate the index
 2. *parseQuery*, turns the user query input into a parsed query as an array of char pointers
array), separated by space
 3. *query*, takes the parsed query and returns a sorted array of counter objects, each containing a document_id and url. Inside *query*, a new counter set *result* is created and merged with the result of each *andsequence* in a query
 4. *andsequence*, returns the result of intersection between two counter sets


 
### 7. Major data structures
`struct counter`: a single counter object that contains document_id and url  
`struct twocountersets`: contains two counter sets

Four helper modules provide data structures:

  1. ***hashtable*** of inverted index mapping from words to (documentID, count) pairs
  2. ***counters*** for each word containing (document_id count) pairs
  3. ***webpage*** which contains the URL, the depth at which it was fetched, and the html of a webpage
  4. ***set*** of countersets (indirectly used by hashtable)
 

### Testing plan

- *Integration testing*.  In `testing.sh`, asssemble the querier and test it as a whole. Examine the query results. 

	* Ensure the number of matched page files is correct
	* Ensure that matched page files are listed in decreasing ranking order
	* Ensure that the information for each page file is printed correctly, one per line, as decribed in the output section

A sampling of tests that should be run:

-  Test the querier with no argument, one argument, and more than two arguments.

-  Test the querier with invalid first argument: pageDirectory does not exist and pageDirectory is not a crawler-produced directory

-  Test the querier with invalid second argument: indexfilename does not exist and exists but is not writable

-  Test the querier with incorrect query inputs containing special characters and having operators at incorrect positions

-  Test the querier with correct query inputs that have different combinations of "and", "or" operators

-  Test the querier with correct query generated by `fuzzquery`


