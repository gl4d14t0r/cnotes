#ifndef NOTE_H
#define NOTE_H

#include <time.h>

typedef struct 
{
	unsigned id;
	char *text;
	char date_created[64];
} note_t;

note_t *create_note(char *text, unsigned available_id, time_t *time);
void display_note(note_t *note);

#endif
