/* 
 * Data Structure
 * 
 * hashtable.c - the 'hashtable' module
 *
 * see hashtable.h for more information.
 *
 * Zitong Wu, January 30, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "memory.h"
#include "jhash.h"
#include "set.h"


/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef struct hashtable {
  set_t **array;
  int length;
} hashtable_t; 

/**************** global functions ****************/
/* that is, visible outside this file */
/* see hashtable.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */

/**************** hashtable_new ****************/
/* see hashtable.h for description */
hashtable_t *
hashtable_new(const int num_slots)
{
  hashtable_t *ht = count_malloc(sizeof(hashtable_t));

  if (ht != NULL && num_slots > 0) {
    ht->array = count_calloc(num_slots, sizeof(set_t*));
    ht->length = num_slots;
    if (ht->array != NULL) {
      return ht;
    }
  } 
  return NULL; // if any error
}

/**************** hashtable_insert ****************/
/* see hashtable.h for description */
bool 
hashtable_insert(hashtable_t *ht, const char *key, void *item)
{
  if (ht != NULL && key != NULL && item != NULL) {
    // hash the key
    unsigned long hashed_key = JenkinsHash(key, ht->length);

    // check if key already exits, check the corresponding hash slot
    if (set_find(ht->array[hashed_key], key) != NULL) {
      // if key exists
      #ifdef MEMTEST
        count_report(stdout, "After set_insert");
      #endif
      return false;
    }

    // if key does not exist
    if (hashed_key >= 0) {
      // create a new set if set does not exist at the hashed key
      if (ht->array[hashed_key] == NULL) { 
        ht->array[hashed_key] = set_new();
      }
      // make a copy of the given key
      char *key_copy = count_malloc(strlen(key) + 1); 
      strcpy(key_copy, key);
      // insert the item to the set corresponding to the hashed key
      bool insert_bool = set_insert(ht->array[hashed_key], key_copy, item);
      count_free(key_copy);
      #ifdef MEMTEST
        count_report(stdout, "After set_insert");
      #endif
      return insert_bool;
    }
  }
  
  #ifdef MEMTEST
    count_report(stdout, "After hashtable_insert");
  #endif

  // if any parameter is NULL or any error
  return false;
}

/**************** hashtable_find ****************/
/* see hashtable.h for description */
void *
hashtable_find(hashtable_t *ht, const char *key)
{
  if (ht != NULL && key != NULL) {
    // hash the key
    unsigned long hashed_key = JenkinsHash(key, ht->length);
    void *result = set_find(ht->array[hashed_key], key);
    if (result != NULL) {
        return result;
    }       
  }

  // if set is NULL, key is NULL, or key is not found
  return NULL;
}

/**************** hashtable_print ****************/
/* see hashtable.h for description */
void 
hashtable_print(hashtable_t *ht, FILE *fp, 
                     void (*itemprint)(FILE *fp, const char *key, void *item))
{
  if (fp != NULL) {
    if (ht != NULL) {
      for (int i = 0; i < ht->length; i++) {
        // print this node
        if (itemprint != NULL) { // print the node's item 
          set_print(ht->array[i], fp, itemprint); 
        }
        fputs("\n", fp);
      }
    } else {
      fputs("(null)", fp);
    }
  } 
}

/**************** hashtable_iterate ****************/
/* see hashtable.h for description */
void 
hashtable_iterate(hashtable_t *ht, void *arg,
                       void (*itemfunc)(void *arg, const char *key, void *item))
{
  if (ht != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (int i = 0; i < ht->length; i++) {
      set_iterate(ht->array[i], arg, itemfunc); 
    }
  }   
}

/**************** hashtable_delete ****************/
/* see hashtable.h for description */
void 
hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item))
{
  if (ht != NULL) {
    for (int i = 0; i < ht->length; i++) { 
      if (itemdelete != NULL) {                // if possible...
        set_delete(ht->array[i], itemdelete);  // delete the set at the hash slot
      }
    }
    count_free(ht->array);
    count_free(ht);
  }

  #ifdef MEMTEST
    count_report(stdout, "End of hashtable_delete");
  #endif 
}
