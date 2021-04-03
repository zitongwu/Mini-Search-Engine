# Crawler
### Zitong Wu, Feb. 2021

<br > 

### crawler

The crawler crawls a website and retrieves webpages starting with a specified URL. It parses the initial webpage, extracts all embedded URLs and retrieves those pages, and crawls the pages found at those URLs, in a recursive manner, but limits itself to maxDepth hops from the seed URL and to URLs that are ‘internal’ to the designated server "http://cs50tse.cs.dartmouth.edu/tse/"
 

### Usage
In the crawler directory,   
`make`: to compile crawler  
`make test`:  test crawler on different cases enumerated in testing.sh  
`make valgrind`: valgrind test crawler on a single case, namely     `valgrind ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data 1`  
`make clean`: clean up the crawler directory


### Exit status:

exit with an error message and non-zero exit status if it encounters an unrecoverable error, including:

* error when creating new webpages
* invalid command-line arguments  
* unable to create or write to a file with the page's information to pageDirectory/id
* error when creating or writing the page's information to pageDirectory/id


### Assumptions:

1. The pageDirectory does not contain any files whose name is an integer (i.e., 1, 2, ...)
2. Internal URL: Only those beginning "http://cs50tse.cs.dartmouth.edu/tse/"
3. To be polite, our crawler purposely slows its behavior by introducing a delay, sleeping for one second between fetches
