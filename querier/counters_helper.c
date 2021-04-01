/* 
 * Counter set helper functions used by querier.c
 *
 * see counter.h for more information
 * 
 * Zitong Wu, Feb 21, 2021
 */
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "pagedir.h"
#include "file.h"
#include "word.h"
#include "counters.h"
#include "hashtable.h"
#include "index.h"
#include "file.h"
#include "memory.h"
#include <unistd.h>
#include "filepath.h"



/**************** global types ****************/
struct counter {
  int doc_id; 
  int count;
};

/**************** local types ****************/
struct twocountersets {
  counters_t *ctrs1; 
  counters_t *ctrs2;
};



/********************** FUNCTIONs *******************/
/***************** min ******************************/
/* see counters_helper.h for description */
int 
min(int a, int b)
{
  if (a >= b) {
    return b;
  } else {
    return a;
  }
}



/***************** insertionSort ******************************/
/* see counters_helper.h for description */
void insertionSort(struct counter **array, int num_items)
{ 
  int current = 0;
  int j = 0;
  int i = 0;
  struct counter *temp = NULL;
  for (i = 1; i < num_items; i ++) {
    current = array[i]->count;
    temp = array[i];
    j = i - 1;
    while ((j >= 0) && ((array[j]->count) < current)) {
      array[j + 1] = array[j];
      j = j - 1;
    }
    array[j + 1] = temp;
  }
}



/***************** insertCounterIntoArray ******************************/
/* see counters_helper.h for description */
void insertCounterIntoArray(void *arg, const int key, const int count)
{ 
  struct counter **array = arg;
  struct counter *current_counter = malloc(sizeof(struct counter*));
  current_counter->doc_id = key;
  current_counter->count = count;
  int i = 0;
  while (array[i] != NULL) {
    i ++;
  }
  array[i] = current_counter;
}



/***************** itemCount ******************************/
/* see counters_helper.h for description */
void itemCount(void *arg, const int key, const int count)
{
  int *num_items = arg;

  if (num_items != NULL && key >= 0 && count >= 0) {
    (*num_items)++;
  } else {
    fprintf(stderr, "counters: node is null or key is null or count is null");
  }
}



/***************** numItems ******************************/
/* see counters_helper.h for description */
int numItems(counters_t *ctrs) {
  int num_items = 0;
  counters_iterate(ctrs, &num_items, itemCount);
  return num_items;
}



/***************** sortCounters ******************************/
/* see counters_helper.h for description */
struct 
counter **sortCounters(counters_t *ctrs, int num_items)
{
  struct counter **array = calloc(num_items, sizeof(struct counter*));
  counters_iterate(ctrs, array, insertCounterIntoArray);
  insertionSort(array, num_items);
  return array;
}



/***************** countersIntersectionHelper ******************************/
/* see counters_helper.h for description */
void 
countersIntersectionHelper(void *arg, const int key, const int count)
{
  struct twocountersets *countersets = arg;
  counters_t *result = countersets->ctrs1;
  counters_t *ctrsB = countersets->ctrs2;
  int countA = count;

  // get the count of the same key in setB
  int countB = counters_get(ctrsB, key);
  // Take the mininum of countA and countB as the count of their intersection
  if (min(countA, countB) != 0) {
    counters_set(result, key, min(countA, countB));
  }
}



/***************** countersIntersection ******************************/
/* see counters_helper.h for description */
counters_t*
countersIntersection(counters_t *ctrsA, counters_t *ctrsB)
{
  counters_t *result = counters_new();
  struct twocountersets countersets = {result, ctrsB};
  counters_iterate(ctrsA, &countersets, countersIntersectionHelper);
  return result;
}



/***************** countersUnionHelper ******************************/
/* see counters_helper.h for description */
void countersUnionHelper(void *arg, const int key, const int count)
{
  counters_t *ctrsA = arg;
  int countB = count;
  
  // get the count of the same key in ctrsA
  int countA = counters_get(ctrsA, key);
  if (countA == 0) {
    // not found: set the count value associated with the key  
    counters_set(ctrsA, key, countB);
  } else {
    // add to the existing value
    counters_set(ctrsA, key, countA + countB);
  }
}



/***************** countersUnion ******************************/
/* see counters_helper.h for description */
void countersUnion(counters_t *ctrsA, counters_t *ctrsB)
{
  counters_iterate(ctrsB, ctrsA, countersUnionHelper);
}


