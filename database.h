#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"

/* 
 * Wrapper functions ofread() and ofwrite() will be used in place of fread() and 
 * fwrite() due to its offset compatibility 
 */
size_t ofread(void *ptr, size_t size, size_t nmemb, long offset, FILE *stream);
size_t ofwrite(void *ptr, size_t size, size_t nmemb, long offset, FILE *stream);

void write_to_database(FILE *database, list_node_t *head);
list_node_t *read_from_database(FILE *database);

#endif
