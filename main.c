/*
 * Brian Chrzanowski
 * Wed Mar 15, 2017 05:33
 */
#include <stdio.h>
#include <string.h>
#include "messages.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	void *dummy = isofield_alloc("tmp field", "0800", 0, strlen("0800"), TYPE_NUMERIC);

	/* debugging only */
	unlink("./DUMP"); // delete the dumping file
	int fd = open("./DUMP", O_CREAT | O_WRONLY, 0644);
	write(fd, dummy, isofield_get_memory_size((iso_field *)dummy));

	if (isofield_verify((iso_field *)dummy))
		printf("ISO FIELD IS NOT VALID!");

	/* change the name it */
    int a = isofield_set_name((iso_field *)dummy, "new field");

	printf("writing now... : %d\n", a);
	write(fd, dummy, isofield_get_memory_size((iso_field *)dummy));

	/* change the data it */
    a = isofield_set_data((iso_field *)dummy, "0400", strlen("0400"));

	printf("writing now... : %d\n", a);
	write(fd, dummy, isofield_get_memory_size((iso_field *)dummy));

	/* free the memory that we asked the operating system for... */
	isofield_free(dummy);

	close(fd);
	return 0;
}
