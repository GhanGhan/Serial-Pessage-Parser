#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "messaging.h"

void getID(uint8_t mID);

int main()
{
    uint8_t msgID = 5;
    char *buffer = &msgID;

    int msgC = *buffer;

    //printf("The messgage ID is: %d \n", msgID );
    //printf("The messgage ID (buffer) is: %d \n", *buffer);
    //printf("The messgage ID is: %d \n", msgC );


    char serial[] = {0xF1, 0xBA, 0xC2, 0xD4, '\0'};
    buffer = serial;
    //int i = 0;
    //while(serial[i] != '\0'){
    //    printf("Byte %d from serial: %X \n",i+1, (unsigned char)serial[i]);
    //    printf("Byte %d from buffer: %X \n",i+1, buffer[i]);
    //    i++;
    //}
    
    char *header = malloc(5*sizeof(char));
    char idArray[] = { 0x41, '\0'};
    char *bufferID = idArray;
    header[0] = bufferID[0];
    char reserve[] = {0xA1, 0xA2, 0xA3, '\0'};
    char *bufferReserve = reserve;
    for(int i = 1; i <= 3; i++){
        header[i] = bufferReserve[i-1];
    }
    header[4] = '\0';

    int j = 0;
    bufferReserve[3] = 0xFF;
    printf("This is the header: \t");
    while(header[j] != '\0'){
        printf("%X", (unsigned char)header[j]);
        j++;
    }
    printf("\n");
    char payload[] = {0x11, 0x8D, 0x5C, 0xA3, '\0'};
    char *bufferLoad = payload;

    printf("This is the payload: \t");
    j = 0;
    while(payload[j] != '\0'){
        printf("%X", (unsigned char)payload[j]);
        j++;
    }
    printf("\n");

    printf("Header XOR'd with the payload: \t");
    for(int k = 0; k < 4; k++){
        printf("%X", (unsigned char)(header[k]^payload[k]));
    }
    printf("\n");


    char input[20];
    char *value;
    for(int i = 0; i < 20; i++){
        input[20] = '\0';
    }
    
    printf("Enter an input: ");
    scanf("%x", input);

    printf("This is the first byte: %X \n", (unsigned char)input[0]);
    value = input;
    j = 0;
    printf("This is input: ");
    while(value[j] != '\0'){
        printf("%X", (unsigned char)value[j]);
        j++;
    }

    




    return 0;

}

void getID(uint8_t mID);