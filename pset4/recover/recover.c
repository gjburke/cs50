#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    //Check that number of arguments are correct
    if (argc != 2) {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }
    char *filename = argv[1];
    //Check that file is openable
    FILE *check = fopen(filename, "r");
    if (check == NULL) {
        printf("File could not be opened\n");
        fclose(check);
        return 1;
    }
    fclose(check);

    //Create file pointer for reading
    FILE* rawFile = fopen(filename, "r");

    //Create a space in memory to assign the read values
    BYTE buffer[BLOCK_SIZE];

    //Create tracker for amount of jpgs and if the bytes are in a jpg
    int jpgCount = 0;
    int inJPG = 0;

    //Allocate memory for the jpg's name
    char *nameptr = malloc(sizeof(char) * 8);

    //Instantiate variable for a new file
    FILE *newFile = fopen("temp", "w");

    //Keep iterating over every chunk of bytes until there is no more to read
    while (fread(buffer, 1, BLOCK_SIZE, rawFile) == BLOCK_SIZE)
    {
        //Checking for a new jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] == 0xe0 || buffer[3] / 0xf == 0xf)) {
            //Close previous file
            fclose(newFile);
            //Store formatted string in that memory spot
            sprintf(nameptr, "%03i.jpg", jpgCount);
            //Open a file with that name (only if it opens)
            newFile = fopen(nameptr, "w");
            if (newFile == NULL) {
                printf("Could not create file %i\n", jpgCount);
                continue;
            }
            //Increase the amount of images and indicate we are in jpg now
            jpgCount++;
            inJPG++;
        }
        if (inJPG > 0) {
            fwrite(buffer, 1, BLOCK_SIZE, newFile);
        }
    }
    //Closing files
    fclose(rawFile);
    fclose(newFile);

    //Release memory are using the name to open the file
    free(nameptr);
}