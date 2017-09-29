#include <stdio.h>

#include "linkedlist.h"
#include "note.h"
#include "database.h"

int main(void)
{
	list_node_t *head = NULL;

	time_t clock = time(NULL);

	note_t *note1 = create_note("Hello, world!", 1, &clock);
	note_t *note2 = create_note("Hello, world!", 2, &clock);
	note_t *note3 = create_note("Hello, world!", 3, &clock);

	list_add(&head, note1);
	list_add(&head, note2);
	list_add(&head, note3);

	head = list_sort(&head);

	list_display(head);
	display_note(head->next->note);

	return 0;
}
