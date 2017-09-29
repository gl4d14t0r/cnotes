#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "note.h"

typedef struct list_node_t
{
	struct list_node_t *previous;
	note_t *note;
	struct list_node_t *next;
} list_node_t;

void list_add(list_node_t **head, note_t *note);
void list_remove(list_node_t **head);
void list_insert(list_node_t **head, unsigned position, note_t *note);
void list_destroy(list_node_t *head);
void list_pluck(list_node_t **head, unsigned id);
list_node_t *list_sort(list_node_t **head);
void list_display(list_node_t *head);
size_t list_length(list_node_t *head);

#endif
