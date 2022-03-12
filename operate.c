#include <stdio.h>
#include <stdbool.h>
#include "operate.h"


//size of message components in bytes
const unsigned int start_flag_size = 5;
const unsigned int msg_ID_size = 1;
const unsigned int reserve_size = 3;
const unsigned int separator_size = 1;
const unsigned int checksum_size = 4;
const unsigned int payload_unit_size = 5;
const char start_byte_code = 0xFF;
const char separator_byte_code = 0XFE;

bool isStartFlag(char *buffer){
    int bytes_recieved = read(buffer, start_flag_size);
    if(bytes_recieved != start_flag_size){
        printf("%d bytes recieved, wrong size! \n", bytes_recieved);
        return false;
    }

    for(unsigned int i = 0; i <start_flag_size; i++){
        if(buffer[i] != start_byte_code){
            printf("Wrong byte Value! \n");
            return false;
        }
    }

    printf("This is the start flag! \n");
    return true;
}

void xOR1o2i(char *out, char *in1, char *in2){
    for(int i = 0; i < msg_ID_size+reserve_size; i++){
        out[i] = in1[i]^in2[i];
    }
}

void  xORsrcdst(char *dst, char *src){
    for(int i = 0; i <msg_ID_size+reserve_size; i++){
        dst[i] = dst[i]^src[i];
    }
}

void display(char *data, int bytes){
    for(int i = 0; i < bytes; i++){
        printf("%X", (unsigned char)data[i]);
    }
    printf("\n");
}

void receive(char *buffer, unsigned int count){
    int actual_count = 0;
    while(actual_count == 0){
        actual_count = read(buffer, count);
    }
}