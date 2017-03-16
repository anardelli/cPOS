/* message alloc, free, packing, and unpacking functions */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "messages.h"

#define FIELD_FLAG 0xefbeadde // oh, Intel!
#define INIT_LEN   32         // initial fields, no realloc

/* -------------------------------------------------------------------------- */
/*                  BEGIN THE DEFINITION OF ISOMESSAGES                       */
/* -------------------------------------------------------------------------- */

/*
 * func : isomessage_alloc
 * args : int len
 * out  : struct **isofield
 * use  : this function
 */

iso_field **isomessage_alloc(int len)
{
	// iso_field **data = malloc(sizeof(iso_field *) * (len == 0) ? INIT_LEN : 0);
	iso_field **data = malloc(sizeof(iso_field *) * len);

	if (data) {
		memset(data, 0, sizeof(iso_field *) * len);
	}

	return data; // User checks return type
}

void isomessage_sort(void *base, int message_len)
{
	qsort(base, message_len, sizeof(iso_field), isomessage_comp);
}

/*
 * func : isomessage_add
 * args : iso_field **array, int len, iso_field *input
 * out  : int -> -1 on failure, 0 on success
 * use  : "insert 'input' into 'array'
 *
 *         Makes use of the C standard library's bsearch and qsort functions.
 *         By default, the add function simply adds to an index that is NULL,
 *         and when the user's done ADDING to the message, they NEED TO
 *         call isomessage_sort, and it takes care of ordering the array.
 *
 *         WARNING : len is not the last index of the array, it is the LENGTH
 *         of the array.
 *
 *         Remember to increase len after you add to the list.
 */

int isomessage_add(iso_field **array, int len, iso_field *input)
{
	int i;
	int val = 0;

	for (i = 0; i < len; i++) {
		if (!array[i]) {
			array[i] = input;
			break;
		}
	}

	if (i == len) { // no space in the array
		val = -1;
	} else {
		val = 0;
	}

	return val;
}

/*
 * func : isomessage_remove
 * args : iso_field **array, int len, int field_num
 * out  : int -> -1 on failure, 0 on success
 * use  : Uses bsearch on the array, and if the element is found in the
 *        array, it frees the memory associated, sets the pointer to NULL,
 *        and returns 0.
 *
 *        If the value is not in the array, -1 is returned and no changes
 *        to the list are made.
 */

int isomessage_remove(iso_field **array, int len, int field_num)
{
	int val = 0;
	int i;

	for (i = 0; i < len && array[i]; i++) {
		if (array[i]->field_num == field_num) {
			free(array[i]);
			array[i] = NULL;
		}
	}

	if (i == len) {
		val = -1;
	}

	return val;
}

/*
 * func : isomessage_comp
 * args : ONLY USE FROM THE CONTEXT OF QSORT
 * out  : ONLY USE FROM THE CONTEXT OF QSORT
 * use  : ONLY USE FROM THE CONTEXT OF QSORT
 */
int isomessage_comp(const void *val1, const void *val2)
{
	iso_field *inner1 = *(iso_field **) val1;
	iso_field *inner2 = *(iso_field **) val2;

	return (inner2->field_num < inner1->field_num);
}

/*
 * func : isomessage_print_all
 * args : iso_field **, int len
 * out  : void
 * use  : prints fields in order, assuming the array is sorted
 */

void isomessage_print_all(iso_field **array, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		if (array[i]) {
			isofield_print(array[i]);
		}
	}
}


/*
 * func : isomessage_deep_free
 * args : iso_field **
 * out  : void
 * use  : this function iterates over the entire array of isofields that
 *        the user has, and simply frees each valid one
 *
 *        This free is set up like this because an isomessage itself is
 *        simply a collection of iso_fields. There's no need to store
 *        information about an isomessage itself when simply encoding
 *        data.
 *
 *        All pointers should come from the return type of an alloc function.
 */

void isomessage_deep_free(iso_field **input, int len)
{
	int i;

	if (!input)
		return;

	for (i = 0; i < len; i++)
		if (input[i])
			free(input[i]);

	free(input);
}

/* -------------------------------------------------------------------------- */
/*                  BEGIN THE DEFINITION OF ISOFIELDS                         */
/* -------------------------------------------------------------------------- */

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
 * notes: We assume that the *name string is NULL terminated because of the
 *        use of functions that rely on it.
 *
 *        This function will round the name and the data (the variable data
 *        sizes) to a multiple of 4, for pretty memory dumps.
 */

void *isofield_alloc(char *name, char *data, int field_num,
					 int name_len, int data_len, unsigned int type)
{
	/* we get the length of the variable data size */
	int var_data_size = ++name_len + data_len;

	if (var_data_size % 4 != 0)
		var_data_size += 4 - var_data_size % 4;

	int size = sizeof(iso_field) + var_data_size;

	void *ptr = malloc(size);

	if (ptr) {
		// set the memory all to zero
		memset(ptr, 0, size);

		// set the fields within the header
		iso_field *tmp = ptr;

		tmp->flag       = FIELD_FLAG;
		tmp->size       = size;
		tmp->field_num  = field_num;
		tmp->data_type  = type;
		tmp->name_len   = name_len;
		tmp->data_len   = data_len;

		// copy the data to the regions of memory AFTER the iso_field struct
		char *byte_ptr = ((char *)ptr) + sizeof(iso_field);

		// copy the name first
		strcpy(byte_ptr, name);

		// get the new pointer
		byte_ptr = byte_ptr + tmp->name_len;

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
 *        pointer arithmetic involved with getting out the data from
 *        iso_fields. This allows the data thrown into the 'struct' to be
 *        of variable length.
 */

char *isofield_get_name(iso_field *input)
{
	return ((char *)input) + sizeof(iso_field);
}

char *isofield_get_data(iso_field *input)
{
	return ((char *)input) + sizeof(iso_field) + input->name_len;
}

long isofield_get_memory_size(iso_field *input)
{
	if (input)
		return input->size;
	return 0;
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

/*
 * func : isomessage_print
 * args : iso_field **, int len
 * out  : void
 * use  : prints fields in order
 */

void isofield_print(iso_field *input)
{
	if (input) {
		printf("field_num : %d, datatype : %d name : '%s', data : '%s'\n",
				input->field_num,
				input->data_type,
				isofield_get_name(input),
				isofield_get_data(input)
				);
	}
}
















/* -------------------------------------------------------------------------- */
/*                    BEGIN OTHER UTILITY FUNCTIONALITY                       */
/* -------------------------------------------------------------------------- */
