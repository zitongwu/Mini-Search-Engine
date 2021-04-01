/* 
 * Counter set helper functions used by querier.c
 *
 * Zitong Wu, Feb 21, 2021
 */


#ifndef COUNTERS_HELPER_H
#define COUNTERS_HELPER_H


#include <stdio.h>
#include <stdlib.h>
#include "counters.h"
#include "counters.h"



/**************** global structs ****************/
struct counter {
  int doc_id; 
  int count;
};



/********************** FUNCTIONs *******************/
/***************** min ******************************/
/* Returns the mininum of a and b
 */ 
int min(int a, int b);



/***************** insertionSort ******************************/
/* Sort an array of counter objects (struct defined at the top of this file)
 * in decreasing order
 * 
 * Note: sort the array in place, not by creating an additional array
 * 
 * Reference: https://www.geeksforgeeks.org/insertion-sort/
 */
void insertionSort(struct counter **array, int num_items);



/***************** insertCounterIntoArray ******************************/
/* Helper function to pass in counters_iterate()
 * Insert a counter object (struct defined at the top of this file) into an array of counter objects
 */
void insertCounterIntoArray(void *arg, const int key, const int count);



/***************** itemCount ******************************/
/* Helper function for numItem()
 * Count the total number of items in a counter set
 */
void itemCount(void *arg, const int current, const int count);



/***************** numItems ******************************/
/* Returns the number of items in a counter set
 * Returns 0 if the counter set is NULL
 */
int numItems(counters_t *ctrs);



/***************** sortCounters ******************************/
/* Returns a sorted array of counter objects (struct defined at the top of this file)
 * in decreasing order
 * 
 * The caller is responsible for freeing the malloc'd space of the returned item
 */
struct counter **sortCounters(counters_t *ctrs, int num_items);



/***************** countersIntersectionHelper ******************************/
/* Helper function for countersIntersection()
 * 
 * If a key exists in both sets, take the mininum of the counts of both sets
 * as the count of their intersection
 */
void countersIntersectionHelper(void *arg, const int current, const int count);



/***************** countersIntersection ******************************/
/* Returns the intersection of two counter sets: 
 * the set of items that both sets have
 * 
 * This function doesn't alter the two counter sets
 * 
 * The caller is responsible for freeing the malloc'd space of the returned item
 */
counters_t *countersIntersection(counters_t *ctrsA, counters_t *ctrsB);



/***************** countersUnionHelper ******************************/
/* Helper function for countersUnion()
 *
 * Consider one (key, count) pair for insertion into the other counter set(A).  
 * If the other counter(A) set does not contain the item, add the (key, count) pair to the other counter set(A)
 * otherwise, update the other counter set(A)'s key with sum of count values of both counter sets.  
 */ 
void countersUnionHelper(void *arg, const int key, const int count);



/***************** countersUnion ******************************/
/* Merge the second counter set into the first counter set
 *
 * The second counter set is unchanged
 * 
 * Adapted from Prof. David Kotz's code  
 */ 
void countersUnion(counters_t *ctrsA, counters_t *ctrsB);






#endif

