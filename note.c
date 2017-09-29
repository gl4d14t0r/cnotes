#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "note.h"

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
