/* 
 * index.h 
 *
 * Utility functions shared by the crawler, indexer, and querier:
 *    loadIndexfile: load the index from an index file into an hashtable of
 *                   inverted-index mapping from word to pairs of (document_id count)
 *    saveIndexfile: create a new file and write a hashtable of inverted index
 *                    to that file in the format described below
 * 
 * Exit with 2 if file pointer is null  
 * 
 * Assumptions: 
 *     The content of the index file follows the following format:
 *     one line per word, one word per line
 *     each line provides the word and one or more (docID, count) pairs, in the format
 *     word docID count [docID count]... 
 *     where word is a string of lower-case letters
 *     where docID is a positive non-zero integer
 *     where count is a positive non-zero integer
 *     where the word and integers are separated by spaces.
 * 
 * Zitong Wu, Feb 15, 2021
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"


/***************** loadIndexfile ******************************/
/* load the index from an index file into an hashtable of
 * inverted-index mapping from each word to a counterset that 
 * contains pairs of (document_id count)
 *
 * Returns a hashtable of inverted-index mapping from word to pairs of (document_id count)
 *    
 * Print an error message and returns NULL if the index filename is not readble
 * 
 * Assumptions for the index file: see header comment.
 */
hashtable_t *loadIndexfile(char *filename);


/***************** saveIndexfile ******************************/
/* create a new file and write a hashtable of inverted index
 * to that file in the format specified in the header comment
 *
 * Print an error message and exit with 2 if the newly created file is not writable
 * 
 * Assumptions for the index file: see header comment.
 */
void saveIndexfile(hashtable_t *index, char *filename);

#endif 

