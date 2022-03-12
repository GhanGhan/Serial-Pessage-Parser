Assumptions:
1. The separator bytes following the the 4 bytes of data was sent as one package.
2. Since it was stated that after start flag was recieved the full message will follow (asynchornously)
and there was no indication anything besides the checksum bytes could be the wrong value, the value of 
the separator bytes were not checked for correctness.
3. Since it was stated that the data is sent in little-endian order, the order of the bytes for each 
payload (minus the separator byte) was reversed before being placed into the message for 
"process_message()" and calculating the correct checksum value.


Program notes:
1. "main()" is in serialParser.c.
2. Serial port helper functions placed in messaging.h.
3. Defintions for Serial port helper functions are in messaging.c, used to test code functionality
by using scanf() to get hexidecimal values from the terminal.
4. Declarations and defintions for constants, functions for polling for bytes and XOR'ing are in
operate.h and operate.c respectively.

gcc -o <program-name> serialParser.c operate.c messaging.c

Example: gcc -o parser serialParser.c operate.c messaging.c
