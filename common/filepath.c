/* 
 * filepath.c 
 *
 * see filepath.h for more information.
 *
 * Zitong Wu, Feb 8, 2021
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "memory.h"

/**************** fullFilePath ****************/
/* see filepath.h for description */
char *fullFilePath(char *pageDirectory, int document_id)
{
  char *filepath = assertp(malloc(200 * sizeof(char)), "malloc for filepath returns NULL\n");
  strcpy(filepath, pageDirectory);
  char *string_id = assertp(malloc(10 * sizeof(char)), "malloc for string_id returns NULL\n");
  sprintf(string_id, "%d", document_id);
  strcat(filepath, "/");
  strcat(filepath, string_id);
  free(string_id);
  return filepath;
}


/**************** isnewline ****************/
/* see filepath.h for description */
int isnewline(int c) 
{ 
  return (c == '\n'); 
}
