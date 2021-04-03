/* 
 * indextest.c - test program for index.c located in the common folder
 *
 * test the following two functions:
 *    loadIndexfile: load the index from an oldIndexFilename into an inverted-index data structure
 *    saveIndexfile: create a file newIndexFilename and write the index to that file in the format described below
 *    (newIndexfilename should be idential to the oldIndexfilename, except the order in which the lines appear)
 *
 * Usage: ./indextest oldIndexFilename newIndexFilename
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "hashtable.h"
#include "counters.h"

static void itemdelete(void *item);

int 
main(const int argc, char *argv[])
{
  hashtable_t *index = NULL;

  // check the number of arguments
  if (argc != 3) {                                            
  printf("usage: ./indextest oldIndexFilename newIndexFilename\n");
  exit(1);
  }
  
  // load the old indexfile
  index = loadIndexfile(argv[1]);
  if (index != NULL) {
    // save to a new indexfile
    saveIndexfile(index, argv[2]);
  } else {
    printf("loadIndexfile returns NULL\n");
  }
  hashtable_delete(index, itemdelete);
}


/***************** itemdelete ******************************/
/* helper function to pass in the hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item))
 * 
 * Free all memory allocated for a counter set by calling counters_delete(counters_t *ctrs)
 */
static void 
itemdelete(void *item)
{
  counters_t *ctrs = item;
  counters_delete(ctrs);
}
