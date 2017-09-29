#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "linkedlist.h"
#include "note.h"
#include "database.h"

#define DATABASE_NAME "/.cnotesdb"
#define FILE_PATH_SIZE 255

typedef struct 
{
	unsigned id;
	char *text;
	char date_created[64];
} note_t;

typedef struct list_node_t
{
	struct list_node_t *previous;
	note_t *note;
	struct list_node_t *next;
} list_node_t;


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
		fprintf(stderr, "Error: cannot sort an empty list.\n");
		exit(1);
	}

	list_node_t *new_head = NULL;
	list_node_t *iterator = *head;
	list_node_t *new_iterator = new_head;
	unsigned position_number = 0;

	while (iterator->next != NULL) {
		if (new_head == NULL)
			list_insert(&new_head, position_number, iterator->note);
		else {
			while (new_iterator->next != NULL) {
				if (iterator->note->id <= new_iterator->note->id)
					list_insert(&new_head, position_number, iterator->note);
				else {
					new_iterator = new_iterator->next;
					position_number++;
					if (new_iterator->next == NULL)
						list_insert(&new_head, position_number, iterator->note);
				}
			}
		}
		iterator = iterator->next;
		new_iterator = new_head;
		position_number = 0;
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

note_t *create_note(char *text, unsigned available_id, time_t *time)
{
	note_t *new_note = malloc(sizeof(note_t));
	struct tm *timeinfo = localtime(time);
	char date_created_buffer[64];

	sprintf(date_created_buffer, "%d/%d/%d", timeinfo->tm_mday, timeinfo->tm_mon + 1, 1900 + timeinfo->tm_year);

	new_note->text = text;
	strcpy(new_note->date_created, date_created_buffer);
	new_note->id = available_id;

	return new_note;
}

void display_note(note_t *note)
{
	printf("%d  %s  %s\n", note->id, note->date_created, note->text);
}

void USAGE(void)
{
	printf("USAGE:\n");
	printf("cnotes -a <text>    Adds note to database\n");
	printf("cnotes -r <id> 	    Removes note from database\n");
	printf("cnotes -l 	    Lists all notes in database\n");
	printf("cnotes -d 	    Destroys entire database\n");
}

int main(int argc, char **argv)
{
	char option;
	char file_path[FILE_PATH_SIZE]; /* change file path buffer size at will, 255 is default */

	time_t clock = time(NULL);

	strcpy(file_path, getenv("HOME"));
	strncat(file_path, DATABASE_NAME, strlen(DATABASE_NAME));

	FILE *database = fopen(file_path, "w+");
	list_node_t *head_of_database = read_from_database(database);
	
	while ((option = getopt(argc, argv, "ardlh")) != -1) {
		switch (option) {
		case 'a':
			if (argc != 3) 
				USAGE();
			else {
				note_t *note = create_note(argv[2], list_length(head_of_database) + 1, &clock);
				list_add(&head_of_database, note);
				head_of_database = list_sort(&head_of_database);
			}
			break;
		case 'l':
			if (argc != 2)
				USAGE();
			else 
				list_display(head_of_database);	
			break;
		case 'r':
			if (argc != 3)
				USAGE();
			else {
				list_pluck(&head_of_database, atoi(argv[2]));
				head_of_database = list_sort(&head_of_database);
			}
			break;
		case 'd':
			if (argc != 2) 
				USAGE();
			else {
				char confirmation;
				printf("Are you sure you want to delete the entire database? [y/n]\n");
				scanf("%c", &confirmation);

				if (confirmation == 'y')
					list_destroy(head_of_database);
				else 
					break;
			}
			break;
		case 'h':
		default:
			USAGE();
			break;
		}
	}
	write_to_database(database, head_of_database);
	fclose(database);

	return 0;
}
