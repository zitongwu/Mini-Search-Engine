# test for the crawler module
# Zitong Wu, January 31, 2021
#!/bin/bash

mkdir data
######################### incorrect arguments
######## incorrect number of arguments
# no argument
./crawler

# one argument
./crawler http://cs50tse.cs.dartmouth.edu/tse/ 

# two arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/ data

# more than three arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/ data 2 hello


######## invalid first argument
# first argument seedURL pointing to a non-existent server
./crawler http://heyheyhey.edu data 1

# first argument seedURL pointing to a non-internal server
./crawler http://cs50tse.cs.dartmouth.edu data 1

# first argument seedURL pointing to a valid server but non-existent page
./crawler http://cs50tse.cs.dartmouth.edu/tse/index.html data 1


######## invalid second argument
# second argument pageDirectory does not exist
./crawler http://cs50tse.cs.dartmouth.edu/tse/ verycool 1


######## invalid third argument
# third argument maxDepth not a number
./crawler http://cs50tse.cs.dartmouth.edu/tse/ data hello

# third argument maxDepth less than 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/ data -5





######################### correct arguments
######## different depths of the tse letter site
######## this site has cycles and duplicate pages, so it  is good for testing
max depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data 0

# max depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data 1

# max depth 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data 2

# max depth 3
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data 3

# max depth 4
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data 4

# max depth 5
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data 5


####### repeat with a different seed page in the same site as above
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/C.html data 3


####### tse wikipedia
mkdir wikipedia

# tse wikipedia at depth 0
mkdir wikipedia/depth0
./crawler http://cs50tse.cs.dartmouth.edu/tse//wikipedia/index.html wikipedia/depth0 0

# tse wikipedia at depth 1
mkdir wikipedia/depth1
./crawler http://cs50tse.cs.dartmouth.edu/tse//wikipedia/index.html wikipedia/depth1 1

# tse wikipedia at depth 2 takes too long to run
# you may uncomment the code below to test it
# I did not test and include it in the testing.out file
# mkdir wikipedia/depth2
# ./crawler http://cs50tse.cs.dartmouth.edu/tse//wikipedia/index.html wikipedia/depth2 2
