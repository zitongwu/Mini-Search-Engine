/* 
 * index.c
 *
 * see index.h for more information.
 * 
 * Zitong Wu, Feb 15, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "file.h"
#include "hashtable.h"
#include "counters.h"


/***************** itemPrint ******************************/
/* helper function to pass into keyItemPrint
 *
 * Write one counter (document_id count) onto a line of a file
 * 
 * Exit with 2 if file pointer is null
 */
static void itemPrint(void *arg, const int key, const int count)
{
  FILE *fp = arg;
  if (fp != NULL) {
    fprintf(fp, "%d %d ", key, count);
  } else {
    printf("file pointer null\n");
    exit(2);
  }
}

/***************** keyitemPrint ******************************/
/* helper function to pass into hashtable_iterate
 *
 * Write a key word and its counterset onto a line of a file:
 * for example: 
 *      word docID1 count1 docID2 count2...
 * 
 * Wxit with 2 if file pointer is null
 */
static void keyItemPrint(void *arg, const char *key, void *item)
{
  FILE *fp = arg;
  if (fp != NULL) {
    counters_t *ctrs = item;
    fprintf(fp, "%s ", key);
    counters_iterate(ctrs, fp, itemPrint);
    fprintf(fp, "\n");
  } else {
    printf("file pointer null\n");
    exit(2);
  }
}

/**************** saveIndexfile ****************/
/* see index.h for description */
void saveIndexfile(hashtable_t *index, char *filename)
{
  FILE *fp = NULL;
  if ((fp = fopen(filename, "w"))) {
    hashtable_iterate(index, fp, keyItemPrint);
    fclose(fp);
  } else {
    printf("The indexFileName %s is not writable\n", filename);
    exit(2);
  }
}

/**************** loadIndexfile ****************/
/* see index.h for description */
hashtable_t *loadIndexfile(char *filename)
{
  int num_lines = -1;
  int num_slots = -1;
  int document_id = -1;
  int count = -1;
  char word[100] = "";
  hashtable_t *index = NULL;
  FILE *fp = NULL;

  if ((fp = fopen(filename, "r"))) {
    num_lines = lines_in_file(fp);
    num_slots = num_lines / 0.75;
    index = hashtable_new(num_slots);
    while ((fscanf(fp, "%s", word)) == 1) {
      // for each word, create a new counterset
      counters_t *ctrs = counters_new();
      while ((fscanf(fp, "%d %d", &document_id, &count)) == 2) {
        // save each [document_id count] pair to the counterset
        if (!counters_set(ctrs, document_id, count)) {
          printf("error when setting count value %d to document_id %d\n", document_id, count);
        }
      }
      // insert the counterset of the word into the hashtable of inverted index
      if (!hashtable_insert(index, word, ctrs)) {
        printf("error when inserting key word %s to index\n", word);
      }
    }
    fclose(fp);
    return index;
  } else {
    printf("The indexFileName %s is not readable\n", filename);
    return NULL;
  }
}



