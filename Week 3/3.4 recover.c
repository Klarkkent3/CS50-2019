#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }
    FILE *inptr = fopen (argv[1], "r");
    if (inptr == 0)
    {
        fprintf(stderr, "Could not open file %s.\n", argv[1]);
        return 2;
    }

    //define file for images
    FILE *outptr = NULL;

    // size of each block that we will read
    const int block_size = 512;

    //temporary storage
    unsigned char buffer[block_size];

    //quantity of recovered images
    int counter = 0;

    //for filename. 8 - because maximum three digits, one dot, three letters (jpg) and one /0
    char filename[8];

    // search through all blocks until JPG is found
    while (fread(buffer, block_size, 1, inptr) == 1)
    {
        // find beggining of the JPG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer [2] == 0xff && buffer [3] > 0xdf && buffer [3] < 0xF0)
        {   
            //start writing our JPGS
            sprintf(filename, "%0.3i.jpg", counter);
            outptr = fopen(filename, "w");
            fwrite(buffer, block_size, 1, outptr);
            counter += 1;

        }
        //it's meen that new JPG is not started and we continue writing previous JPG
        else if (counter > 0)
        {
            fwrite(buffer, block_size, 1, outptr);
        }
    }
    fclose(inptr);
    fclose(outptr);
}






