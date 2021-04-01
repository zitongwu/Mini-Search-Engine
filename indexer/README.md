# Indexer
### Zitong Wu
<br > 

### indexer

The indexer reads the files produced by the TSE crawler, builds an index, and writes that index to a file.


### Usage
**Make sure to run `make` in the common and libcs50 folder before compiling the indexer. Or simply run `make all` in the lab5 home folder.**

In the indexer directory,   

* `make` or `make all`: to compile both indexer and indextest 

* `make indexer`: to compile only indexer

* `make indextest`: to compile only indextest
 
* `make test`: test `indexer` and `indextest`

	* test `indexer` on different cases enumerated in testing.sh 
	* call `indextest` to create a newIndexfile from an oldIndexfile and compare the two files for differences


* `make valgrind`: valgrind test indexer and indextest

```
valgrind ./indexer ../crawler/data.depth1 valgrindIndexfile
valgrind ./indextest valgrindIndexfile cp_valgrindIndexfile
```

`make clean`: clean up the indexer directory

### Test Data
Test data is located in ../crawler/data/depth0, ../crawler/data/depth1, ../crawler/data/depth2, each containing 1 page file, 4 page files, and 84 page files

### Test Result
see `testing.out` and index files:  
`depth0IndexFile` `depth0IndexFile.sorted`  
`depth1IndexFile` `depth1IndexFile.sorted`  
`oldIndexFile` `oldIndexFile.sorted` (this is effectively the indexfile for ../crawler/data/depth2)  
`newIndexFile` `newIndexFile.sorted` (`newIndexFile.sorted and `oldIndexFile.sorted` should match exactly)  
`valgrindIndexfile` `cp_valgrindIndexfile`