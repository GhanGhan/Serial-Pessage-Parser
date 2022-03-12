#ifndef _operate_h
#define _operate_h

#include <stdbool.h>
#include "messaging.h"

//size of message components in bytes
extern const unsigned int start_flag_size;
extern const unsigned int msg_ID_size;
extern const unsigned int reserve_size;
extern const unsigned int separator_size;
extern const unsigned int checksum_size;
extern const unsigned int payload_unit_size;

//Values of the start flag and the sep
extern const char start_byte_code;
extern const char separator_byte_code;

/**
 * Checks to see if the start flag has been sent from the serial port
 * 
 * Returns true if it has, returns false if it has not
 */
bool isStartFlag(char *buffer);

/**
 * Applies the XOR operation on in1 and in2.  Stores the value in out.
 */
void xOR1o2i(char *out, char *in1, char *in2);

/**
 * Applies the XOR operation on src and dst.  Stores the value in dst.
 */
void xORsrcdst(char *dst, char *src);

/**
 * Outputs the the specified amount of bytes of data onto the console
 */
void display(char *data, int bytes);

/**
* Polls the serial port attempting to read up to count number 
* of bytes from the serial port into the buffer.
*
* Returns the number of bytes actually read.
*/
void receive(char *buffer, unsigned int count);

#endif