#include <string.h>
#include <stdlib.h>
#include "messages.h"


/*
 * func : set
 * args : struct ISOmsg *, int, char *, int
 * out  : int
 * use  : set a message within a given ISOmsg
 */

int ISOmsg_set_field (struct ISOmsg *ptr, int index, char *data, int data_size)
{
	int return_val = 0;

	if (ptr) {

	}
}












/*
 * func : make_message
 * args : void
 * out  : struct ISOmsg *
 * use  : get a new ISOmsg ptr
 */

struct ISOmsg *make_message()
{
	struct ISOmsg *ptr = malloc(sizeof(struct ISOmsg));

	/* set the data */
	ptr->current = 0;
	ptr->max     = 256;
	ptr->index = NULL;
	ptr->data  = NULL;

	return ptr;
}

/*
 * func : free_message
 * args : struct ISOmsg *
 * out  : void
 * use  : this will perform a deep free, that is, it will free all data
 *        utilized within the structure
 */

void free_message(struct ISOmsg *ptr)
{
	if (ptr->data) {
		free(ptr->data);
	}

	if (ptr->index) {
		free(ptr->index);
	}

	if (ptr) {
		free(ptr);
	}
}
