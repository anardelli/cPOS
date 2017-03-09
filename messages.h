/*
 * messages.h
 *
 * this message passing interface is described in here:
 *
 *		http://jpos.org/doc/proguide-draft.pdf
 *	
 * messages are essentially a bit mapping affair. So, using the defines
 * below, a simple '|' operation can easily set a map.
 *
 * TODO::
 *
 *		create shorter macros
 */

// Message Type Indicator Bit Fields
enum HEADER_MESSAGE_MTI {
HEADER_MTI_VERSION_1987              = 0,
HEADER_MTI_VERSION_1993              = 1,
HEADER_MTI_VERSION_2003              = 2,

HEADER_MTI_VERSION_RESERVED_ISO_LOW  = 3,
HEADER_MTI_VERSION_RESERVED_ISO_HIGH = 7,

HEADER_MTI_VERSION_NATIONAL          = 8,
HEADER_MTI_VERSION_PRIVATE           = 9
};

// Message Class
enum HEADER_MESSAGE {
HEADER_MESSAGE_CLASS_RESERVED_0      = 0,
HEADER_MESSAGE_CLASS_AUTHORIZATION   = 1,
HEADER_MESSAGE_CLASS_FINANCIAL       = 2,
HEADER_MESSAGE_CLASS_FILE_UPDATE     = 3,
HEADER_MESSAGE_CLASS_CHRGBAK         = 4,
HEADER_MESSAGE_CLASS_RECONCILIATION  = 5,
HEADER_MESSAGE_CLASS_ADMINISTRATIVE  = 6,
HEADER_MESSAGE_CLASS_FEECOLLECT      = 7,
HEADER_MESSAGE_CLASS_NETMANAGE       = 8,
HEADER_MESSAGE_CLASS_RESERVED_9      = 9
};

// Message Function
enum HEADER_MESSAGE_FUNC {
HEADER_MESSAGE_FUNCTION_REQUEST           = 0,
HEADER_MESSAGE_FUNCTION_REQUEST_RESPONSE  = 1,
HEADER_MESSAGE_FUNCTION_ADVICE            = 2,
HEADER_MESSAGE_FUNCTION_ADVICE_RESPONSE   = 3,
HEADER_MESSAGE_FUNCTION_NOTIFICATION      = 4,
HEADER_MESSAGE_FUNCTION_RESERVED_ISO_LOW  = 5,
HEADER_MESSAGE_FUNCTION_RESERVED_ISO_HIGH = 9
};

// Transaction Originator
enum HEADER_TRANS_ORIGINATOR {
HEADER_TRANS_ORIGINATOR_ACQUIRER           = 0,
HEADER_TRANS_ORIGINATOR_ACQUIRER_REPEAT    = 1,
HEADER_TRANS_ORIGINATOR_CARD_ISSUER        = 2,
HEADER_TRANS_ORIGINATOR_CARD_ISSUER_REPEAT = 3,
HEADER_TRANS_ORIGINATOR_OTHER              = 4,
HEADER_TRANS_ORIGINATOR_OTHER_REPEAT       = 5,
HEADER_TRANS_ORIGINATOR_RESERVED_ISO_LOW   = 6,
HEADER_TRANS_ORIGINATOR_RESERVED_ISO_HIGH  = 9
};

/* the above enumerations will only be needed in testing messages */





/* 
 * this structure can be utilized to create messages. Also, at the time of
 * this structure's creation, the maximum message length is 9999, which is
 * included in the range of 0 <-> 2^31-1 (It's between 0 and 65536)
 */

#define START_SIZE 16

typedef struct ISOmsg_field { /* these fields are meant to be internal */
	short num;
	short length;
	char  *data;
} ISOmsg_field;

typedef struct ISOmsg {
	short type;			/* this is just the MTI value */
	int current;			/* current size */
	int max;			/* current max allocated size */

	struct ISOmsg_field *data;	/* pointer to array */
} ISOmsg;

