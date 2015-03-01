#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[3])
{
    if(argc != 3){
        printf("Invalid arguments!");
        exit(1);
    }

    FILE * input_ptr = fopen(argv[1], "rb");
    FILE * output_ptr = fopen(argv[2], "wb");
    int bytes_toRead = 44;
    unsigned char b_array[44];
    unsigned char * first_ff = b_array;


    int byt_read = (int)fread(first_ff, sizeof(char), bytes_toRead, input_ptr);
    if (byt_read != bytes_toRead){
        printf("Error reading the file!");
    }else{
        fwrite(first_ff, sizeof(char), byt_read, output_ptr);
    }

    fclose(output_ptr);
    output_ptr = fopen(argv[2], "ab");


    int new_fread;
    short left;
    short right;
    short combined;
    short buffer_shorts[44];

    while((new_fread = fread(buffer_shorts, sizeof(short), 44, input_ptr)) > 0){
        int i;
        for(i=0; i < new_fread; i+=2){
            left = buffer_shorts[i];
            right = buffer_shorts[i+1];
            combined = (left - right)/2;
            buffer_shorts[i] = combined;
            buffer_shorts[i+1] = combined;

        }
        fwrite(buffer_shorts, sizeof(short), new_fread, output_ptr);
    }

    fclose(input_ptr);
    fclose(output_ptr);

    return 0;
}
