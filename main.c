/*
 * Brian Chrzanowski
 * Wed Mar 15, 2017 05:33
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "messages.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int compare(const void *a, const void *b)
{
	iso_field *val1 = *(iso_field **)a;
	iso_field *val2 = *(iso_field **)b;

	return (val2->field_num < val1->field_num);
}

int main(int argc, char **argv)
{
	int len = 10;

	char *a = "MTI";
	char *b = "Duck Duck Goose";
	char *c = "Hello  ";

	char *aa = "0800";
	char *bb = "623411";
	char *cc = "Yams";

	/*               name data type   length: name, data,    TYPE */
	void *dummy1 = isofield_alloc(a, aa, 0, strlen(a), strlen(aa), TYPE_CHAR);
	void *dummy2 = isofield_alloc(b, bb, 6, strlen(b), strlen(bb), TYPE_CHAR);
	void *dummy3 = isofield_alloc(c, cc, 2, strlen(c), strlen(cc), TYPE_CHAR);

	iso_field **arr = isomessage_alloc(len);
	isomessage_add(arr, len, dummy3);
	isomessage_add(arr, len, dummy2);
	isomessage_add(arr, len, dummy1);

	/* debugging only */
	unlink("./DUMP"); // delete the dumping file
	int fd = open("./DUMP", O_CREAT | O_WRONLY, 0644);
	write(fd, dummy2, isofield_get_memory_size((iso_field *)dummy2));
	write(fd, dummy1, isofield_get_memory_size((iso_field *)dummy1));
	write(fd, dummy3, isofield_get_memory_size((iso_field *)dummy3));

	isomessage_print_all(arr, len);
	qsort(arr, 3, sizeof(iso_field *), compare);
	// isomessage_sort(arr, 5);
	printf("\n");
	isomessage_print_all(arr, len);

	/* free the memory that we asked the operating system for... */
	isomessage_deep_free(arr, 10);

	close(fd); // close the file
	return 0;
}
