/* 
 * word.c 
 *
 * see word.h for more information.
 *
 * Zitong Wu, Feb 15, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void normalizeWord(char *word)
{
  for(char *c = word; *c != '\0' ; c++){
    *c = tolower(*c);
  }
}
