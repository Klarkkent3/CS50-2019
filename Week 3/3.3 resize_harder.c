// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    float f = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    if (f > 100.0 || f < 0.0)
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
    int ini_padding = (4 - (ini_biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // determine new width, new height and new padding
    bi.biWidth = floor(ini_biWidth * f);
    bi.biHeight = floor(ini_biHeight * f);
    int new_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // determine new biSizeImage, bfSize
    bi.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + new_padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    double WidthCoef = (double) ini_biWidth / (double) bi.biWidth;
    double HeightCoef = (double) ini_biHeight / (double) bi.biHeight;
    
    // designate temporary storage for scanline
    RGBTRIPLE triple[ini_biWidth * sizeof(RGBTRIPLE)];
    int TempScanline = -1;

    // commence building rows of new image
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // define how row of old image correspond to the row of new image 
        int row = i * HeightCoef;
        
        // determine scalnline from old image that correspond to new image
        if (TempScanline != row)
        {
            // We move cursor from the begginning of file to the position of current row
            fseek(inptr, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (((sizeof(RGBTRIPLE) * ini_biWidth) + ini_padding) * row), SEEK_SET);
            fread(triple, sizeof(RGBTRIPLE), ini_biWidth, inptr);
            TempScanline = row;
        }
        
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // determine tier from old image that correspond to new image
            int tier = j * WidthCoef;
            fwrite(&triple[tier], sizeof(RGBTRIPLE), 1, outptr);
        }
        // add new padding
        for (int b = 0; b < new_padding; b++)
        {
                fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
