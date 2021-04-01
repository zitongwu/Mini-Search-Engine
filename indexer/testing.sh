# testing.sh - test for the indexer module
# Zitong Wu, Feburary 15, 2021
#!/bin/bash

######################### incorrect arguments
######## incorrect number of arguments
# no argument
./indexer

# one argument
./indexer ../crawler/data/depth1 

# more than two arguments
./indexer ../crawler/data/depth1 indexFile 3


######## invalid first argument
# first argument pageDirectory does not exist 
./indexer foo/ indexFile

# first argument pageDirectory is not a Crawler-produced directory
mkdir dir
./indexer dir/ indexFile
rm -rf dir


######## invalid second argument
# second argument indexFileName exists but is not writable
touch indexFile2
chmod -w ./indexFile2
./indexer ../crawler/data/depth1 indexFile2
rm -f indexFile2



######################### correct arguments
#### data/depth0 folder contains only 1 saved page file
./indexer ../crawler/data/depth0 depth0IndexFile
awk -f indexsort.awk depth0IndexFile > depth0IndexFile.sorted

#### data/depth1 folder contains 4 saved page files
./indexer ../crawler/data/depth1 depth1IndexFile
awk -f indexsort.awk depth1IndexFile > depth1IndexFile.sorted

#### data/depth2 folder contains 84 saved page files
# create an indexfile with the name "oldIndexFile"
./indexer ../crawler/data/depth2 indexFile
awk -f indexsort.awk oldIndexFile > indexFile.sorted
head -20 indexFile.sorted

# call indextest.c to create newIndexFile from oldIndexFile
./indextest oldIndexFile newIndexFile
awk -f indexsort.awk newIndexFile > newIndexFile.sorted

# check if there is any difference between sorted oldIndexFile and newIndexFile
diff oldIndexFile.sorted newIndexFile.sorted
