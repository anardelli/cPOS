/*
 * Brian Chrzanowski
 * Wed Mar 15, 2017 05:33
 *
 * An example for the cPOS presentation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "messages.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MESSAGE_LENGTH 200

int main(int argc, char **argv)
{
	int i, j;
	char *message_data[] = {
		"0200",
		"1500",
		"1206041200",
		"000001",
		"12340001",
		"0840"
	};

	char *message_name[] = {
		"MTI",
		"FIELD_CODE",
		"FIELD_SETTLEMENT",
		"CONVERSION_RATE",
		"LOOK, WE CAN DO SPACES TOO!",
		"Lower case too. Isn't ascii great?"
	};

	iso_field **data = isomessage_alloc(MESSAGE_LENGTH);

	for (i = 5, j = 0; i > 0; i--, j++) {
		iso_field *tmp = 
			isofield_alloc(message_name[i], message_data[i], i,
					strlen(message_name[i]), strlen(message_data[i]),
					TYPE_CHAR);

		if (isomessage_add(data, MESSAGE_LENGTH, tmp)) {
			// message array is full
			break;
		}
	}

	/* debugging only */
	unlink("./DUMP"); // delete the dumping file
	int fd = open("./DUMP", O_CREAT | O_WRONLY, 0644);

	isomessage_print_all(data, MESSAGE_LENGTH);
	printf("\n");
	qsort(data, j, sizeof(iso_field *), isomessage_comp);
	// isomessage_sort(data, j);

	for (i = 0; i < j; i++) { // memory dumping example
		write(fd, data[i], isofield_get_memory_size(data[i]));
	}

	isomessage_print_all(data, MESSAGE_LENGTH);

	/* free the memory that we asked the operating system for... */
	isomessage_deep_free(data, MESSAGE_LENGTH);

	close(fd); // close the file
	return 0;
}

