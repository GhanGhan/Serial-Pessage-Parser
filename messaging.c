#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "messaging.h"


int read(char *buffer, unsigned int count){
    char input[20];
    int num = 0;
    for(int i = 0; i < 20; i++){
        input[20] = '\0';
    }

    printf("Enter an input: ");
    scanf("%llx", input);

    while(input[num]!= '\0'){
        num++;
    }
    for(int i = num-1; i>= 0; i--){
        buffer[num-i-1] = input[i];
    }
    return num;
}


void process_message(char *buffer, unsigned int message_id){

}

int get_message_size_from_message_id(int message_id){
    return message_id;
}