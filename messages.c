/* message alloc, free, packing, and unpacking functions */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "messages.h"

#define FIELD_FLAG 0xefbeadde // oh, Intel!
#define INIT_LEN   32         // initial fields, no realloc

/*
 * func : isomessage_alloc
 * args : int len
 * out  : struct **isofield
 * use  : use this to create an array of iso fields.
 */
iso_field *isomessage_alloc(int len)
{
	iso_field *data = malloc(sizeof(iso_field *) * len);
	return data;
}


/*
 * func : iso_field_alloc
 * args : char *name, char *data, int field_num, 
 *        int data_len (in bytes), int type
 *
 * out  : void *
 * use  : This function is used to allocate memory for an ISO 8583 field.
 *        By passing parameters to the function, a malloc is performed,
 *        the variable data is copied as requested to the user, and
 *        stored at the END of an iso_field struct.
 *
 *        Other functions within this library can be used to access the
 *        data by performing pointer arithmetic for the user.
 *
 * notes: we assume that the *name string is NULL terminated because of the
 *        use of functions that rely on it.
 */

void *isofield_alloc(
		char *name, char *data, int field_num, int data_len, unsigned int type)
{
	void *ptr = malloc(sizeof(iso_field) + strlen(name) + 1 + data_len);

	if (ptr) {
		int name_len = strlen(name);

		// set the memory all to zero
		memset(ptr, 0, sizeof(iso_field) + name_len + 1 + data_len);

		// set the fields within the header
		iso_field *tmp = ptr;

		tmp->flag       = FIELD_FLAG;
		tmp->field_num  = field_num;
		tmp->name_len   = strlen(name);
		tmp->data_type  = type;
		tmp->data_len   = data_len;

		// copy the data to the regions of memory AFTER the iso_field struct
		char *byte_ptr = ((char *)ptr) + sizeof(iso_field);

		// copy the name first
		strcpy(byte_ptr, name);

		// get the new pointer
		byte_ptr = byte_ptr + tmp->name_len + 1;

		memcpy(byte_ptr, data, data_len);
	}

	return ptr;
}

/*
 * func : isofield_free
 * args : iso_field *input
 * out  : void
 * use  : this is just a wrapper function around 'free'
 */
void isofield_free(void *input)
{
	if (input) {
		free(input);
	}
}

/*
 * func : isofield_verify
 * args : iso_field *input
 * out  : int
 * use  : pass in a pointer, and using the flag, it will determine if the
 *        field is correct. WARNING: It is assumed that if the field is
 *        not the FLAG defined at the top, it is impossible to know if the
 *        data in the field is corrupt or not.
 *
 *        0 on valid flag, -1 on error
 */
int isofield_verify(iso_field *input)
{
	return (input->flag == FIELD_FLAG) ? 0 : -1;
}

/*
 * func : get_data_ptr, get_name_ptr, get_field_memory_size
 * args : void
 * out  : char * where appropriate
 * use  : Use these functions to get data out of individual fields. While
 *        a message itself is simply an array of iso_field ptrs, there's
 *        pointer arithmetic involved within getting out the data from
 *        iso_fields. This allows the data thrown into the 'struct' to be
 *        of variable length.
 */

char *isofield_get_name(iso_field *input)
{
	return ((char *)input) + sizeof(iso_field);
}

char *isofield_get_data(iso_field *input)
{
	return ((char *)input) + sizeof(iso_field) + input->name_len + 1;
}

long isofield_get_memory_size(iso_field *input)
{
	return sizeof(iso_field) + input->name_len + 1 + input->data_len;
}

/*
 * func : isofield_set_{name, data}
 * args : set_data and set_name use char *,
 *        but set_type takes in an unsigned integer
 *
 *        All functions require an iso_field * as well
 * out  : int. returns -1 on error
 * use  : pass in data and the various length functions. If the function
 *        returns -1, then it has failed and the user should use predefined
 *        functions to create another iso field, set it inside of the isomsg,
 *        and free this one.
 */

int isofield_set_data(iso_field *input, char *input_data, int input_data_len)
{
	if (input_data_len > input->data_len) {
		return -1;
	} else {
		// zero out the memory
		memset((char *)input + sizeof(iso_field) + input->name_len, 0,
				input_data_len);

		// copy the data
		memcpy((char *)input + sizeof(iso_field) + input->name_len + 1,
				input_data, input_data_len);
	}

	return 0;
}

int isofield_set_name(iso_field *input, char *input_name)
{
	// get the length of the name
	long namelen = strlen(input_name);

	if (sizeof(iso_field) + input->data_len + namelen > 
			isofield_get_memory_size(input)) {
		return -1;
	} else {
		// set the existing string space to 0
		memset((char *)input + sizeof(iso_field), 0, input->name_len);

		// copy the new name
		strcpy((char *)input + sizeof(iso_field), input_name);
	}

	return 0;
}
