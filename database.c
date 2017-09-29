#include <stdio.h>
#include <stdlib.h>

#include "database.h"
#include "linkedlist.h"
#include "note.h"

size_t ofread(void *ptr, size_t size, size_t nmemb, long offset, FILE *stream)
{
	if ((fseek(stream, offset, SEEK_SET)) != 0) 
		return 0;
	else
		return fread(ptr, size, nmemb, stream);
}

size_t ofwrite(void *ptr, size_t size, size_t nmemb, long offset, FILE *stream)
{
	if ((fseek(stream, offset, SEEK_SET)) != 0)
		return 0;
	else
		return fwrite(ptr, size, nmemb, stream);
}

void write_to_database(FILE *database, list_node_t *head)
{
	int i = 0;
	while (head != NULL) {
		ofwrite(head, sizeof(note_t), 1, sizeof(note_t) * i, database);
		head = head->next;
		i++;
	}
}

static _Bool file_is_empty(FILE *fp)
{
	size_t fsize;

	if (fp != NULL) {
		fseek(fp, 0L, SEEK_END);
		fsize = ftell(fp);

		if (fsize == 0)
			return 1;
	}

	return 0;
}

list_node_t *read_from_database(FILE *database)
{		
	list_node_t *new_head = NULL;

	if (file_is_empty(database))
		return new_head;

	note_t *new_note = malloc(sizeof(note_t));

	int i = 0;
	while ((ofread(new_note, sizeof(note_t), 1, sizeof(note_t) * i, database)) != 0) {
		list_add(&new_head, new_note);	
		i++;
	}

	return new_head;
}
