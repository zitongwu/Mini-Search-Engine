/* 
 * Data Structure
 * 
 * counter.c - the 'counter' module
 *
 * see counter.h for more information.
 *
 * Zitong Wu, January 30, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct counternode {
  int key;
  int count;                 
  struct counternode *next;       // link to next node
} counternode_t;

/**************** global types ****************/
typedef struct counters {
  struct counternode *head;       // head of the linked list of the counter set
} counters_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see counters.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static counternode_t *counternode_new(const int key);


/**************** counters_new ****************/
/* see counters.h for description */
counters_t *
counters_new(void)
{
  counters_t *counters = count_malloc(sizeof(counters_t));

  if (counters == NULL) {
    return NULL;              // error allocating counters
  } else {
    // initialize contents of counter set structure
    counters->head = NULL;
    return counters;
  }
}

/**************** counters_add ****************/
/* see counters.h for description */
int 
counters_add(counters_t *ctrs, const int key)
{
  if (ctrs != NULL && key >= 0) {
    // search the given key in ctrs
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next){
      // if key exists, increment its counter by 1
      if (node->key == key) { 
        node->count ++;
        #ifdef MEMTEST
          count_report(stdout, "After counters_add");
        #endif
        return node->count;
      } 
    }
    // if key does not exits, allocate a new node to be added to the linked list of the counter set
    counternode_t *new = counternode_new(key);
    if (new != NULL) {
      // add it to the head of the linked list of the counter set
      new->next = ctrs->head;
      ctrs->head = new;
      #ifdef MEMTEST
        count_report(stdout, "After counters_add");
      #endif
      return new->count;
    } 
  }
  #ifdef MEMTEST
    count_report(stdout, "After counters_add");
  #endif 
  // if any error
  return 0; 

}

/**************** counternode_new ****************/
/* Allocate and initialize a counternode */
static counternode_t *
counternode_new(const int key)
{
  counternode_t *node = count_malloc(sizeof(counternode_t));

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    node->key = key;
    node->count = 1;
    node->next = NULL;
    return node;
  }
}

/**************** counters_get ****************/
/* see counters.h for description */
int 
counters_get(counters_t *ctrs, const int key)
{
  if (ctrs != NULL && key >= 0) {
    // search the given key in ctrs
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next){
      if (node->key == key) { 
        return node->count;
      }
    }
  }
  // if ctrs is NULL or if key is not found
  return 0;
}

/**************** counters_set ****************/
/* see counters.h for description */
bool 
counters_set(counters_t *ctrs, const int key, const int count){
  if (ctrs != NULL && key >= 0 && count >= 0) {
    // search the given key in ctrs
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next){
      // if key exists, update the counter value with the given count value
      if (node->key == key) { 
        node->count = count;
        #ifdef MEMTEST
          count_report(stdout, "After counters_set");
        #endif
        return true;
      } 
    }
    // if key does not exist, add the key to the counter set
    counternode_t *new = counternode_new(key);
    if (new != NULL) {
      // initialize the new counter with the given count value
      new->count = count;
      // add it to the head of the linked list of the counter set
      new->next = ctrs->head;
      ctrs->head = new;
      #ifdef MEMTEST
        count_report(stdout, "After counters_set");
      #endif 
      return true;
    } 
  }
  #ifdef MEMTEST
    count_report(stdout, "After counters_set");
  #endif 
  // if ctrs is NULL, if key < 0 or count < 0, if out of memory, or if any error
  return false; 
}

/**************** counters_print ****************/
/* see counters.h for description */
void 
counters_print(counters_t *ctrs, FILE *fp){
  if (fp != NULL) {
    if (ctrs != NULL) {
      fputc('{', fp);
      for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
        // print this node
        fprintf(fp, "%d=%d,", node->key, node->count);
      }
      fputc('}', fp);
      fputc('\n', fp);
    } else {
      fputs("(null)\n", fp);
    }
  }
}

/**************** counters_iterate ****************/
/* see counters.h for description */
void 
counters_iterate(counters_t *ctrs, void *arg, 
                      void (*itemfunc)(void *arg, 
                                       const int key, const int count))
{
  if (ctrs != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->count); 
    }
  }  
}

/**************** counters_delete ****************/
/* see counters.h for description */
void 
counters_delete(counters_t *ctrs)
{
  if (ctrs != NULL) {
    for (counternode_t *node = ctrs->head; node != NULL;) { 
      counternode_t *next = node->next; // remember what comes next
      count_free(node);                 // free the node
      node = next;                      // and move on to next
    }
    count_free(ctrs);                   // free the counter set itself
  }

#ifdef MEMTEST
  count_report(stdout, "End of counters_delete");
#endif
}

