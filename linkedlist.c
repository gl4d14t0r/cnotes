#include <stdio.h>
#include <stdlib.h>

#include "note.h"
#include "linkedlist.h"

void list_add(list_node_t **head, note_t *note)
{
	list_node_t *new = malloc(sizeof(list_node_t));
	new->note = note;
	new->next = *head;
	if (*head != NULL)
		(*head)->previous = new;
	*head = new;
}

void list_remove(list_node_t **head)
{
	if (*head == NULL) {
		fprintf(stderr, "Error: cannot remove from empty list.\n");
		exit(1);
	}

	list_node_t *temp = (*head)->next;
	free(*head);
	*head = temp;
	if (*head != NULL)
		(*head)->previous = NULL;
}

void list_insert(list_node_t **head, unsigned position_number, note_t *note)
{
	if (*head == NULL || position_number == 0) {
		list_add(head, note);
		return;
	}

	list_node_t *new = malloc(sizeof(list_node_t));
	list_node_t *position = *head;

	for (int i = 0; i < position_number; i++)
		position = position->next;

	if (position->next == NULL) {
		position->next = new;
		new->previous = position;
	}

	new->note = note;
	new->next = position;
	position->previous = new;
	position->previous->next = new;
	new->previous = position->previous;
}

void list_pluck(list_node_t **head, unsigned id)
{
	if (*head == NULL) {
		fprintf(stderr, "Error: can't pluck from empty list.\n");
		exit(1);
	}

	list_node_t *to_destroy = *head;

	while (to_destroy->note->id != id)
		to_destroy = to_destroy->next;

	to_destroy->previous->next = to_destroy->next;
	to_destroy->next->previous = to_destroy->previous;
	free(to_destroy);
}

void list_destroy(list_node_t *head)
{
	list_node_t *to_destroy;

	while (head != NULL) {
		to_destroy = head;
		head = head->next;
		free(to_destroy);
	}
}

list_node_t *list_sort(list_node_t **head)
{
	if (*head == NULL) {
		fprintf(stderr, "Cannot sort an empty list.\n");
		exit(1);
	}

	list_node_t *new_head = NULL;
	list_node_t *iterator = (*head)->next;
	list_node_t *new_iterator = new_head;
	unsigned position_number = 0;

	list_insert(&new_head, position_number, iterator->note);
	while (iterator != NULL) {
		while (new_iterator != NULL) {
			if (iterator->note->id <= new_iterator->note->id) {
				list_insert(&new_head, position_number, iterator->note);
				break;
			} else {
				if (new_iterator->next == NULL) {
					list_insert(&new_head, position_number, iterator->note);
					break;
				} else {
					new_iterator = new_iterator->next;
					position_number++;
				}
			}
		}
		new_iterator = new_head;
		position_number = 0;
		iterator = iterator->next;
		
	}

	list_destroy(*head);
	return new_head;
}

void list_display(list_node_t *head)
{
	printf("Notes:\n");

	if (head == NULL) {
		printf("N/A\n");
		return;
	}

	while (head != NULL) {
		display_note(head->note);
		head = head->next;		
	}
}

size_t list_length(list_node_t *head)
{
	size_t count = 0;

	while (head != NULL) {
		head = head->next;
		count++;
	}

	return count;
}
