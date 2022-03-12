#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "messaging.h"

//size of message components in bytes
const unsigned int start_flag_size = 5;
const unsigned int msg_ID_size = 1;
const unsigned int reserve_size = 3;
const unsigned int seperator_size = 1;
const unsigned int checksum_size = 4;
const unsigned int payload_unit_size = 5;
const char start_byte_code = 0xFF;
const char seperator_byte_code = 0XFE;


bool isStartFlag(char *buffer);
void xOR1o2i(char *out, char *in1, char *in2);
void xORsrcdst(char *dst, char *src);
void display(char *data, int bytes);

int main()
{
    
    while(1)
    {
        //poll serial port for the start flag
        int msg_size = 0;
        bool start_flag = false;
        char *header = malloc((msg_ID_size + reserve_size)*sizeof(char));
        char *buffer = malloc((start_flag_size)*sizeof(char));
        int num_payloads;
        bool validData = true;
        int num_recieved = 0;

        if(isStartFlag(buffer)) //if buffer is the start flag, start parsing the message!
        {
            //get the the size of the message
            printf("Get the message ID\n");
            read(buffer, msg_ID_size);
            msg_size = get_message_size_from_message_id((int)*buffer);
            printf("The message size is %d\n", msg_size);
            //}
            //place message ID into the header
            header[0] = buffer[0];
            //calculate the number of repeated payload units
            num_payloads = (msg_size -(msg_ID_size + reserve_size + seperator_size + checksum_size + seperator_size))/payload_unit_size;

            //get the reserved bytes
            printf("Get the Reserved Bytes\n");
            read(buffer, reserve_size);
            //place reserve bytes into the header
            for(int i = 1; i <= reserve_size; i++){
                header[i] = buffer[i-1];
            }
            printf("This is the header: ");
            display(header, 4);

            //get the seperator
            printf("Get the Separator\n");
            read(buffer, seperator_size);

            //get data
            char *message = malloc((start_flag_size+msg_ID_size+reserve_size+seperator_size+num_payloads*payload_unit_size+checksum_size+ seperator_size)*sizeof(char));
            char *data =  malloc(num_payloads*payload_unit_size*sizeof(char));
            char *cmpchecksum = malloc(checksum_size*sizeof(char));
            for(int i = 0; i < num_payloads; i++){
                printf("Get the payload!\n");
                read(buffer, payload_unit_size);
                printf("This is the current payload: ");
                display(buffer, payload_unit_size);

                int s = 0; int t = 0;
                for(int j = 0; j < payload_unit_size - 1; j++){
                    s = i*payload_unit_size+j; t = payload_unit_size -j -1;
                    data[i*payload_unit_size+j] = buffer[payload_unit_size - 2 - j];
                    //data sent in little endian -> reverse the order
                }
                data[(i+1)*payload_unit_size - 1] = buffer[payload_unit_size - 1];
                printf("This is the current payload REVERSED: ");
                char *temp = &data[i*payload_unit_size];
                display(temp, 5);
                

                if(i == 0){
                    xOR1o2i(cmpchecksum, header, buffer);
                }
                else xORsrcdst(cmpchecksum, buffer);
                printf("This is the current checksum value: ");
                display(cmpchecksum, 4);
                
                
            }//end getting payloads

            //get checksum
            printf("This is the serial checksum: ");
            read(buffer, checksum_size);
            display(buffer, 4);

            //Compare calculated and recieved checksum
            for(int i= 0; i < 4; i++){
                if(buffer[0]!=cmpchecksum[0]){
                    validData = false;
                    printf("Calculated and recieved checksum are not equal!  Discard data.\n");
                    break;
                }

            }

            

            if(validData){
                printf("Get the Separator\n");
                read(buffer, seperator_size);
                printf("This data is valid!!");
                int index = 0;
                int i = 0;
                for(i = index; i < start_flag_size; i++){
                    message[i] = start_byte_code;
                }
                index += start_flag_size;
                for(int i = index; i < index + msg_ID_size+reserve_size; i++){
                    message[i] = header[i-index];
                }
                
                index += msg_ID_size+reserve_size;
                message[index + seperator_size -1] = seperator_byte_code;
                
                index += seperator_size;
                int f = 0;
                for(i = index; i < index + num_payloads*payload_unit_size; i++ ){
                    f = i-index;
                    message[i] = data[i-index];
                }

                index += num_payloads*payload_unit_size;
                for(i = index; i < index + checksum_size; i++){
                    message[i] = cmpchecksum[i-index];
                }
                index += checksum_size;

                message[index] = seperator_byte_code;
                index++;

                printf("This is the entire parsed message: ");
                display(message, index);
                //process_message(buffer, m)
            }


        }


    }// end infinite while loop
    
    

   return 0;
}

bool isStartFlag(char *buffer){
    if(read(buffer, start_flag_size) != start_flag_size){
        printf("Wrong size! \n");
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