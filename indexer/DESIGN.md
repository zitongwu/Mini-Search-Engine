# Indexer
<br > 

## DESIGN SPEC
### Zitong Wu
<br > 
<br > 

### User Interface
The indexer's only interface with the user is on the command-line; it must always have three arguments.

```bash
$ ./indexer pageDirectory indexFilename
```

- `pageDirectory` is the directory in which files produced by the crawler are saved
- `indexFilename` is the filename to save the index

For example, to index the crawler produced files stored in `../crawler/data/depth1`, and to save the index to `oldIndexfile`, we would use this command-line:

``` bash
$ ./indexer ../crawler/data/depth1 oldIndexfile
```

### Inputs and outputs

*Input:* the inputs are command-line parameters and the `pageDirectory` where the crawler-produced page files are saved by the first command-line parameter and the `indexFilename` where the index will be saved by the third. See the User Interface above.

*Output:* Indexer will scan each page file in the `pageDirectory` and build an inverted-index data structure mapping from words to (documentID, count) pairs, wherein each count represents the number of occurrences of the given word in the given document.

 *     The content of the index file follows the following format:
 *     one line per word, one word per line
 *     each line provides the word and one or more (docID, count) pairs, in the format
 *     word docID count [docID count]... 
 *     where word is a string of lower-case letters
 *     where docID is a positive non-zero integer
 *     where count is a positive non-zero integer
 *     where the word and integers are separated by spaces.

### Functional decomposition into modules

* We anticipate the following modules or functions for `indexer.c`:

 1. *main*, which parses arguments and calls `buildInvertedIndex` and `saveIndexFile` 
 2. *buildInvertedIndex*, which builds an inverted-index data structure mapping from words to (documentID, count) pairs

* We anticipate the following modules or functions for `index.c` in the common direcotry :
 3. *saveIndexfile*, which creates a new file and write a hashtable of inverted index to that file in the format described above
 4. *loadIndexfile*, which loads the index from a file into an hashtable of inverted-index mapping from word to pairs of (document_id count)
 5. *word*, which converts all letters of a word to lowercase

* We also anticipate `indextest.c` to test the two functions in `index.c`, namely: `saveIndexFile` and `loadIndexFile`. It should load the index from the oldIndexFilename into an inverted-index data structure, create a file newIndexFilename and write the index to that file, in the format described above.

* 
And some helper modules that provide data structures:

  1. ***hashtable*** of inverted index mapping from words to (documentID, count) pairs
  2. ***counters*** for each word containing (document_id count) pairs

### Pseudo code for logic/algorithmic flow

The indexer will run as follows:

1. start from a command line as shown in the User Interface
2. parse the command line, validate parameters
3. build the hashtable of inverted index

	for each file in `pageDirectory`:
	
	3.1 extract the url, depth and html from the file  
	3.2 construct a webpage from the retrieved url, depth and html  
	3.3 for each word extracted from the webpage:
	
		3.3.1 check if word length is greater than 2
		3.3.2 normalize the word
		3.3.3 increment the count for the current file id in the counterset for the word

				3.3.3.1 search for the word as a key in the hashtable of inverted index
				3.3.3.2 if word exists in the hashtable of inverted index, increment the count for the current file id in the word's counterset
				3.3.3.3 if word does not exist in the hashtable of inverted index, create a new counterset for the word, add the file id to the counterset, initialize its count value to 1, and insert the word's counterset to the hashtable of inverted index
	3.4 return the hashtable of inverted index
4. create a new file and write the hashtable of inverted index to that file 
5. free the memory of the hashtable of inverted index

**normalize the word** means to convert all letters to lowercase 


### Dataflow through modules
 1. *main* parses parameters and passes them to the `buildInvertedIndex`
 2. *buildInvertedIndex* builds a hashtable of inverted index that contains words as keys and countersets as items for the keys. Each counterset contains pairs of (file_id count)
 3. *saveIndexfile* creates a new file and writes the hashtable of inverted index to that file
 4. *loadIndexfile* recreates a hashtable of inverted index from an index file previously saved in the format above (see ouput). Note: `loadIndexFile` is not called by `indexer.c` but will be tested by `indextest.c`

### 7. Major data structures

Two helper modules provide data structures:

  1. ***hashtable*** of inverted index mapping from words to (documentID, count) pairs
  2. ***counters*** for each word containing (document_id count) pairs
  3. ***webpage*** which contains the URL, the depth at which it was fetched, and the html of a webpage
  4. ***set*** of countersets (indirectly used by hashtable)


### Testing plan

- *Unit testing*.  A small test program `indextest.c` to test `loadIndexfile` and `saveIndexfile`  
- *Integration testing*.  In` testing.sh`, asssemble the indexer and test it as a whole. Examine the index file produced by the indexer. 

	* Ensure there are no duplicate keys for the same word
	* Ensure that that there there are no duplicate keys for the same document_id
	* Ensure that the count value for the corresponding document_id is correct
	* Ensure the index file is correctly formatted, as specified in the output section

- In `indextest.c`, call `loadIndexfile` to load the index from an old indexfile, then call `saveIndexfile` to save the index into a new file. Check if the oldIndexfile matches exactly with the newIndexfile when their lines are sorted.

A sampling of tests that should be run:

-  Test the indexer with no argument, one argument, and more than two arguments.

-  Test the indexer with invalid first argument: pageDirectory does not exist and pageDirectory is not a crawler-produced directory

-  Test the indexer with invalid second argument: indexfilename exists but is not writable

-  Test the indexer with a pageDirectory that contains only one file (crawled with depth 0)

-  Test the indexer with a pageDirectory that contains multiple files (crawled with depth 1)

-  Test the indexer with a pageDirectory that contains even more files (crawled with depth 2)

-  Copy an oldIndexFile to a newIndexfile, have them sorted in alphabetical order, and check if they match exactly

