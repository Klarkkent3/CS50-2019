// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    if (n > 100 || n < 1)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // initial Width, Height and padding
    int ini_biWidth = bi.biWidth;
    int ini_biHeight = bi.biHeight;
    int ini_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // determine new width, new height and new padding
    bi.biWidth = bi.biWidth * n;
    bi.biHeight = bi.biHeight * n;
    int new_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // determine new biSizeImage, bfSize
    bi.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + new_padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(ini_biHeight); i < biHeight; i++)
    {
        // read each scanline 'n' time
        for (int row = 0 ; row < n; row++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < ini_biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple multiple 'n' times to outfile
                for (int b = 0; b < n; b++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // add new padding
            for (int b = 0; b < new_padding; b++)
            {
                fputc(0x00, outptr);
            }

            // Move row tracker to the beginning of a scanline
            if (row != (n - 1))
            {
                fseek(inptr, -ini_biWidth * sizeof(RGBTRIPLE), SEEK_CUR);
            }
            
        }
        
        // skip over padding, if any
        fseek(inptr, ini_padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
