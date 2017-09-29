#include <stdio.h>

#include "database.h"
#include "linkedlist.h"

int main(void)
{
	FILE *fp = fopen(".db", "w+");
	
	time_t clock = time(NULL);
  
	list_node_t *head = read_from_database(fp);
	note_t *note = create_note("xd", list_length(head) + 1, &clock);
	list_add(&head, note);
	list_add(&head, note);
	list_add(&head, note);
	list_sort(&head);
	write_to_database(fp, head);
	head = NULL;
	head = read_from_database(fp);
	list_display(head);

	return 0;
}
