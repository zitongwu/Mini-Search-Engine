/* 
 * word.h 
 *
 * Utility function shared by the crawler, indexer, and querier:
 *    normalizeWord: returns a word that converts all the letters of the input word to lower-case
 *    The returned word points to malloc'd space
 *    the caller is responsible for freeing that space
 * 
 * Zitong Wu, Feb 16, 2021
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void normalizeWord(char *word);

#endif 

