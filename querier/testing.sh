# testing.sh - test for the querier module
# Zitong Wu, Feburary 22, 2021
#!/bin/bash


######################### incorrect arguments
######## incorrect number of arguments
# no argument
./querier

# one argument
./querier data/depth2 

# more than two arguments
./querier data/depth2 indexFile 3


######## invalid first argument
# first argument pageDirectory does not exist 
./querier foo/ indexFile

# first argument pageDirectory is not a Crawler-produced directory
mkdir dir
./querier dir/ indexFile
rm -rf dir


######## invalid second argument
# second argument indexFileName does not exist
./querier data/depth1 indexFile2

# second argument indexFileName exists but is not readable
touch indexFile2
chmod -r ./indexFile2
./querier data/depth1 indexFile2
rm -f indexFile2


######################### correct arguments
####### incorrect queries 
# test queries containing special characters and having operators at incorrect positions
# use self-created incorrect query examples
./querier data/depth2 indexFile < incorrectQueries


####### correct queries
# test different combinations of "and", "or" operators 
# use self-created correct query examples
./querier data/depth2 indexFile < correctQueries

# use fuzzquery
./fuzzquery indexFile 10 0 | ./querier data/depth2 indexFile