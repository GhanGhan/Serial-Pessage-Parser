#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "operate.h"



int main()
{
    char *header = malloc((msg_ID_size + reserve_size)*sizeof(char));//will store the message ID and reserve bytes
    char *buffer = malloc(start_flag_size*sizeof(char));//will be used to access the serial port
    int msg_size = 0;
    bool start_flag = false;
    int num_payloads = 0;
    bool validData = true;
    
    while(1)
    {
        msg_size = 0;
        num_payloads = 0;
        start_flag = false;
        validData = true;
        
        //poll serial port for the start flag
        if(isStartFlag(buffer)) //if buffer is the start flag, start parsing the message!
        {
            //get the the size of the message
            printf("Get the message ID\n");
            receive(buffer, msg_ID_size);
            msg_size = get_message_size_from_message_id(*buffer);
            printf("The message size is %d\n", msg_size);
            
            //place message ID into the header
            header[0] = buffer[0];
            //calculate the number of repeated payload units, should be a multiple
            num_payloads = (msg_size -(msg_ID_size + reserve_size + separator_size + checksum_size + separator_size))/payload_unit_size;

            //get the reserved bytes
            printf("Get the Reserved Bytes\n");
            receive(buffer, reserve_size);
            
            //place reserve bytes into the header
            for(int i = 1; i <= reserve_size; i++){
                header[i] = buffer[i-1];
            }
            printf("This is the header: ");
            display(header, 4);

            //get the separator
            printf("Get the Separator\n");
            receive(buffer, separator_size);
            

            //get the data
            char *data =  malloc(num_payloads*payload_unit_size*sizeof(char));
            char *cmpchecksum = malloc(checksum_size*sizeof(char));
            for(int i = 0; i < num_payloads; i++){
                //Acquire the payload
                printf("Get the payload!\n");
                receive(buffer, payload_unit_size);
                
                printf("This is the current payload: ");
                display(buffer, payload_unit_size);

                //data sent in little endian -> reverse the order
                for(int j = 0; j < payload_unit_size - 1; j++){
                    data[i*payload_unit_size+j] = buffer[payload_unit_size - 2 - j];   
                }
                //place separator byte from buffer into data
                data[(i+1)*payload_unit_size - 1] = buffer[payload_unit_size - 1];
                printf("This is the current payload REVERSED: ");
                char *temp = &data[i*payload_unit_size];
                display(temp, 5);
                
                //Calculate correct checksum value
                if(i == 0){
                    xOR1o2i(cmpchecksum, header, buffer);
                }
                else xORsrcdst(cmpchecksum, buffer);
                printf("This is the current checksum value: ");
                display(cmpchecksum, 4);
            }//end getting payloads

            //get checksum
            printf("This is the serial checksum: ");
            receive(buffer, checksum_size);
            display(buffer, 4);

            //Compare calculated and recieved checksum
            for(int i= 0; i < 4; i++){
                if(buffer[0]!=cmpchecksum[0]){
                    validData = false;
                    free(cmpchecksum);
                    free(data);
                    printf("Calculated and recieved checksum are not equal!  Discard data.\n");
                    break;
                }
            }

            char *message = malloc((start_flag_size+msg_ID_size+reserve_size+separator_size+num_payloads*payload_unit_size+checksum_size+ separator_size)*sizeof(char));
            if(validData){
                printf("This data is valid!!");
                printf("Get the Separator\n");
                receive(buffer, separator_size);
                
                int index = 0;
                int i = 0;
                //place start flag into message
                for(i = index; i < start_flag_size; i++){
                    message[i] = start_byte_code;
                }
                //place header into message
                index += start_flag_size;
                for(int i = index; i < index + msg_ID_size+reserve_size; i++){
                    message[i] = header[i-index];
                }
                
                //place separator byte into message
                index += msg_ID_size+reserve_size;
                message[index + separator_size -1] = separator_byte_code;
                
                //place data int message
                index += separator_size;
                for(i = index; i < index + num_payloads*payload_unit_size; i++ ){
                    message[i] = data[i-index];
                }

                //place checksum into message
                index += num_payloads*payload_unit_size;
                for(i = index; i < index + checksum_size; i++){
                    message[i] = cmpchecksum[i-index];
                }

                //place final separator byte into message
                index += checksum_size;
                message[index] = separator_byte_code;
                index++;

                printf("This is the entire parsed message: ");
                display(message, index);
                process_message(message, (unsigned int)header[0]);
                free(message);
                free(cmpchecksum);
                free(data);
            }//end of validate data if statement
        }//end of start_flag if statement
    }// end infinite while loop
    free(header);
    free(buffer);
   return 0;
}
