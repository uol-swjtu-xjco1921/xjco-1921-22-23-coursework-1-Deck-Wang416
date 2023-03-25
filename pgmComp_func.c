#include <stdlib.h>
#include <stdio.h>

#include "pgmComp_func.h"
#include "pgmEcho_func.h"
#include "pgma2b_func.h"
#include "pgmb2a_func.h"

// Compare whether two files have the same logical content
int pgmComp(char *input_file1name, char *input_file2name)
{
    // Open the files
    FILE* inputFile1 = fopen(input_file1name, "r");

    FILE* inputFile2 = fopen(input_file2name, "r");

    struct PGM_Image* img1 = new_img();

    struct PGM_Image* img2 = new_img();
    
    // Validate input files
    open_input_file(inputFile1, input_file1name);

    open_input_file(inputFile2, input_file2name);

    // Read magic number for both images
    img1->magic_number[0] = getc(inputFile1);

    img1->magic_number[1] = getc(inputFile1);

    img2->magic_number[0] = getc(inputFile2);

    img2->magic_number[1] = getc(inputFile2);

    // Read comment lines for both images
    read_comment_line(inputFile1, img1);

    read_comment_line(inputFile2, img2);

    // Read image headers for both images
    read_image_header(inputFile1, input_file1name, img1);

    read_image_header(inputFile2, input_file2name, img2);

    // Allocate memory for the image data of both images
    allocate_image_data(inputFile1, img1);

    allocate_image_data(inputFile2, img2);

    // Read binary data if the magic number indicates a binary PGM file
    if(img1->magic_number[1] == 53)
    {
        int reading_binary = read_binary(inputFile1, input_file1name, img1);

        if(reading_binary != EXIT_NO_ERRORS)

            return reading_binary;
    }

    if(img2->magic_number[1] == 53)
    {
        int reading_binary = read_binary(inputFile2, input_file2name, img2);

        if(reading_binary != EXIT_NO_ERRORS)

            return reading_binary;
    }

    // Read ASCII data if the magic number indicates an ASCII PGM file
    if(img1->magic_number[1] != 53)
    {

        int reading_ascii = read_ascii(inputFile1, input_file1name, img1);

        if(reading_ascii != EXIT_NO_ERRORS)

            return reading_ascii;
    }

    if(img2->magic_number[1] != 53)
    {
        int reading_ascii = read_ascii(inputFile2, input_file2name, img2);

        if(reading_ascii != EXIT_NO_ERRORS)

            return reading_ascii;
    }
    
    // Compare dimensions
    if (img1->width == img2->width && img1->height == img2->height)
    {
        for (int i = 0; i < img1->nImageBytes; i++)
        {
            // Compare pixel values by calculating the normalized gray value for each pixel
            double pixel1 = (double)(img1->imageData[i]) / (double)img1->maxGray;

            double pixel2 = (double)(img2->imageData[i]) / (double)img2->maxGray;

            if (pixel1 != pixel2)
            {
                printf("DIFFERENT\n");

                return EXIT_NO_ERRORS;
            }
        }
        printf("IDENTICAL\n");

        return EXIT_NO_ERRORS;
    }
    else
    {
        printf("DIFFERENT\n");

        return EXIT_NO_ERRORS;
    }
}