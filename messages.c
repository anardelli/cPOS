/* message packing and unpacking functions */

#include <string.h>
#include <stdlib.h>
#include "messages.h"


/*
 * func : set
 * args : struct ISOmsg *, int, char *, int
 * out  : int
 * use  : set a message within a given ISOmsg. IT IS ASSUMED THAT YOU USE
 *        DYNAMIC MEMORY WITHIN THIS FUNCTION
 *
 *        ISO 8583 message fields are a bit peculiar. They can be of any
 *        length (currently supported values are, 'fixed', variable of lengths,
 *        99, 999, and 9999.
 */

int ISOmsg_set_field(struct ISOmsg *ptr, int index, char *data, int data_size)
{
	int return_val = 0;

	if (ptr) {
		if (ptr->current + 1 > ptr->max) {
			/* realloc space */
		}

		/* free the space if used */
		// use bsearch


		/* */
	}

	return return_val;
}






/*
 * func : ISOmsg_alloc
 * args : void
 * out  : struct ISOmsg *
 * use  : get a new ISOmsg ptr. This only sets up the header. 
 *        The ISOmsg_set_field handles not only the allocation of the initial
 *        message field array, but also the resizing
 */

struct ISOmsg *ISOmsg_alloc(int type)
{
	struct ISOmsg *ptr = malloc(sizeof(struct ISOmsg));

	if (ptr) {
		/* set the data */
		ptr->type    = ISOmsg_set_len(type);
		ptr->current = 0;
		ptr->max     = START_SIZE;
		ptr->data    = NULL;
	}

	return ptr;
}




/*
 * func : ISOmsg_set_len
 * args : int
 * out  : int
 * use  : this will convert an integer representing the year standard,
 *        to the corresponding MTI_HEADER value.
 */
static char ISOmsg_set_len(int type)
{
	int val;

	switch (type) {
	case 1987:
		val = 0;
	case 1993:
		val = 1;
	case 2003:
		val = 2;

	default:
		val = -1;

	// HEADER_MTI_VERSION_NATIONAL          = 8,
	// HEADER_MTI_VERSION_PRIVATE           = 9
	}

	return val;
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
