#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


int main(int argc, char * argv[]){
    if(argc < 3){
        printf("Invalid arguments!\n");
        exit(1);
    }
    extern int optind;
    int option;
    int delay = 8000;
    int volume = 4;
    while((option = getopt(argc, argv, "d:v:")) != -1){
        switch (option){
            case 'd':
                delay = atoi(optarg);
                break;

            case 'v':
                volume = atoi(optarg);
                break;
            default:
                printf("Invalid argument!\n");
                exit(1);
        }
    }

    FILE * input_ptr = fopen(argv[optind], "rb");
    FILE * output_ptr = fopen(argv[optind+1], "wb");

    unsigned int array[11];
    int bytes_toRead = 11;

    int byt_read = (int) fread(array, sizeof(int), bytes_toRead, input_ptr);
    if (byt_read != bytes_toRead){
        printf("Error reading the file!\n");
        exit(1);
    }else{
        array[1] = array[1] + (delay * 2);
        array[10] = array[10] + (delay *2);
        fwrite(array, sizeof(int), byt_read, output_ptr);
    }

    fclose(output_ptr);
    output_ptr = fopen(argv[optind+1], "ab");
    
    short * read_buffer =   malloc(sizeof(short)*delay); 
    short * write_buffer = malloc(sizeof(short)*delay); 
    short * echo_buffer = malloc(sizeof(short)*delay); 
    
    if(read_buffer == NULL || write_buffer == NULL || echo_buffer == NULL ){
      perror("Malloc for buffer failed!");
      exit(1); 
    }

    byt_read =  fread(read_buffer, sizeof(short), delay, input_ptr); 
     if (byt_read != delay){ 
        int i; 
      for (i=byt_read; i < delay; i++){ 
    	read_buffer[i] = 0; 
     } 
      for (i=0; i < byt_read; i++){
      	echo_buffer[i] = read_buffer[i]/volume; 
       } 
    }else{
      int i; 
      for (i=0; i < byt_read; i++){ 
      	echo_buffer[i] = read_buffer[i]/volume; 
       } 
      fwrite(read_buffer, sizeof(short), byt_read, output_ptr); 
    } 
    
    int read;
    int last = 0;
    while((read = fread(read_buffer, sizeof(short), delay, input_ptr)) > 0){
      last = read; 
      if (read  != delay){ 
       int a;  
      for (a=read; a < delay; a++){ 
    	read_buffer[a] = 0; 
      }
      }
      int i; 
      for(i=0; i < delay; i++){
	write_buffer[i] = read_buffer[i] + echo_buffer[i]; 
      }
      fwrite(write_buffer, sizeof(short), delay, output_ptr); 
      for (i=0; i < delay; i++){
	echo_buffer[i] = read_buffer[i]/volume; 
      } 
      } 
    fwrite(echo_buffer, sizeof(short), last, output_ptr);
 
    fclose(input_ptr);
    fclose(output_ptr);

    return 0;
}
