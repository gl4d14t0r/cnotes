#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "linkedlist.h"
#include "note.h"
#include "database.h"

#define DATABASE_NAME "/.cnotesdb"
#define FILE_PATH_SIZE 255

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
