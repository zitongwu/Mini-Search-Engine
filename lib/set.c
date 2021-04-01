/* 
 * Data Structure
 * 
 * set.c - the 'counter' module
 *
 * see set.h for more information.
 *
 * Zitong Wu, January 30, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct setnode {
  void *item;
  char *key;              
  struct setnode *next;       // link to next node
} setnode_t;

/**************** global types ****************/
typedef struct set {
  struct setnode *head;       // head of the linked list of the set
} set_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static setnode_t *setnode_new(char *key, void *item);


/**************** set_new ****************/
/* see set.h for description */
set_t *
set_new(void){
  set_t *set = count_malloc(sizeof(set_t));

  if (set == NULL) {
    return NULL;              // error allocating bag
  } else {
    // initialize contents of set structure
    set->head = NULL;
    return set;
  }  
}

/**************** set_insert ****************/
/* see set.h for description */
bool 
set_insert(set_t *set, const char *key, void *item)
{
  if (set != NULL && key != NULL && item != NULL) {
    // check if key already exits
    for (setnode_t *node = set->head; node != NULL; node = node->next){
      // if key exists
      if (strcmp(node->key, key) == 0) { 
        #ifdef MEMTEST
          count_report(stdout, "After set_insert");
        #endif
        return false;
      } 
    }
    // if key does not exist
    // make a copy of the given key
    char *key_copy = count_malloc(strlen(key) + 1); 
    strcpy(key_copy, key);
    setnode_t *new = setnode_new(key_copy, item);
    if (new != NULL) {
      // add it to the head of the linked list of the set
      new->next = set->head;
      set->head = new;
      #ifdef MEMTEST
        count_report(stdout, "After set_insert");
      #endif
      return true;
    } 
  }
  // if any parameter is NULL or any error

#ifdef MEMTEST
  count_report(stdout, "After set_insert");
#endif

  return false;
}

/**************** setnode_new ****************/
/* Allocate and initialize a setnode */
static setnode_t * // not visible outside this file
setnode_new(char *key, void *item)
{
  setnode_t *node = count_malloc(sizeof(setnode_t));

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    node->key = key;
    node->item = item;
    node->next = NULL;
    return node;
  }
}

/**************** set_find ****************/
/* see set.h for description */
void *
set_find(set_t *set, const char *key)
{
  if (set != NULL && key != NULL) {
    // search the given key in set
    for (setnode_t *node = set->head; node != NULL; node = node->next){
      if (strcmp(node->key, key) == 0) { 
        return node->item;
      }
    }
  }
  // if set is NULL, key is NULL, or key is not found
  return NULL;
}

/**************** set_print ****************/
/* see set.h for description */
void 
set_print(set_t *set, FILE *fp, 
               void (*itemprint)(FILE *fp, const char *key, void *item)) 
{
  if (fp != NULL) {
    if (set != NULL) {
      fputc('{', fp);
      for (setnode_t *node = set->head; node != NULL; node = node->next) {
        // print this node
        if (itemprint != NULL) { // print the node's item 
          (*itemprint)(fp, node->key, node->item); 
          fputc(',', fp);
        }
      }
      fputc('}', fp);
    } else {
      fputs("(null)", fp);
    }
  }
}

/**************** set_iterate ****************/
/* see set.h for description */
void 
set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item)) 
{ 
  if (set != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (setnode_t *node = set->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->item); 
    }
  }                    

}

/**************** set_delete ****************/
/* see set.h for description */
void 
set_delete(set_t *set, void (*itemdelete)(void *item)) 
{
  if (set != NULL) {
    for (setnode_t *node = set->head; node != NULL; ) {
      if (itemdelete != NULL) {         // if possible...
        (*itemdelete)(node->item);      // delete node's item
      }
      setnode_t *next = node->next;     // remember what comes next
      count_free(node->key);            // free the key string
      count_free(node);                 // free the node
      node = next;                      // and move on to next
    }

    count_free(set);
  }

#ifdef MEMTEST
  count_report(stdout, "End of set_delete");
#endif
}
