/* messages.h */

/* Begin by defining the type of data that can appear in a field. */
#define TYPE_NUMERIC        0x01
#define TYPE_ALPHA          0x02
#define TYPE_ALPHANUMERIC   0x04
#define TYPE_CHAR           0x08

typedef struct iso_field { // the definition of an iso 8583 field
	int   flag;
	int   field_num;
	int   data_type;
	short name_len;
	short data_len;
	long  size;

	/* 
	 * It is assumed that using pointer arithmetic, one can get to the
	 * NAME and DATA fields. There are functions below that help in getting
	 * these pointers if a user isn't comfortable with pointer arithmetic.
	 */
} iso_field;

/* Utility functions that assist in the use of individual iso_fields.  */
iso_field **isomessage_alloc(int len);
int isomessage_add(iso_field **array, int len, iso_field *input);
int isomessage_remove(iso_field **array, int len, int field_num);
int compmi(const void *val1, const void *val2);
void isomessage_deep_free(iso_field **input, int len);




/* 
 * Utility functions that act have alloc and free, as well
 * as getters and setters
 */

void *isofield_alloc(char *name, char *data, int field_num,
					 int data_len, int name_len, unsigned int type);

void isofield_free(void *input);

char *isofield_get_name(iso_field *input); // getters
char *isofield_get_data(iso_field *input);
long isofield_get_memory_size(iso_field *input);

int isofield_set_name(iso_field *input, char *input_name); // setters
int isofield_set_data(iso_field *input, char *input_data, int input_data_len);

void isofield_print(iso_field *input);
