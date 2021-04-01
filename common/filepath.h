/* 
 * filepath.h 
 *
 * Utility functions shared by the crawler, indexer, and querier:
 *    fullFilePath: construct the full file path with a given pageDirectory and a document_id
 *    isnewline:Stop function for use with freaduntil()
 * 
 * Exit:
 *    2. creating a new webpage failed when calling webpage_new()
 *    4. error when creating or writing to a file with the page's information to pageDirectory/id
 * 
 * Zitong Wu, Feb 24, 2021
 */



#ifndef __FILEPATH_H__
#define __FILEPATH_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************** isnewline ****************/
/* 
 * Stop function for use with freaduntil()
 */
int isnewline(int c);


/***************** fullFilePath ******************************/
/* 
 * Construct the full file path from a given pageDirectory and a document_id
 * for example: pageDirectory/document_id
 * 
 * Returns the full file path stored in malloc'd space
 * the caller is responsible for freeing that space
 */
char *fullFilePath(char *pageDirectory, int document_id);

#endif 

