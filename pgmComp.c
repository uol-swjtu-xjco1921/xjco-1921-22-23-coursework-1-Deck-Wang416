#include <stdlib.h>

#include "pgmComp.h"
#include "handleFile.h"
#include "pgma2b.h"
#include "pgmb2a.h"

/* Compare whether two files have the same logic */
int pgmComp(char *input_file1name, char *input_file2name)
{
    /* Open and check the file format */
    FILE* inputFile1 = fopen(input_file1name, "r");

    FILE* inputFile2 = fopen(input_file2name, "r");

    struct PGM_Image* img1 = new_img();

    struct PGM_Image* img2 = new_img();
    
    open_input_file(inputFile1, input_file1name);

    /* Read magic number */
    img1->magic_number[0] = getc(inputFile1);
    img1->magic_number[1] = getc(inputFile1);

    open_input_file(inputFile2, input_file2name);

    /* Read magic number */
    img2->magic_number[0] = getc(inputFile2);
    img2->magic_number[1] = getc(inputFile2);

    read_comment_line(inputFile1, img1);
    
    read_comment_line(inputFile2, img2);

    read_image_header(inputFile1, input_file1name, img1);

    read_image_header(inputFile2, input_file2name, img2);

    allocate_image_data(inputFile1, img1);

    if(img1->magic_number[1] == 53)
    {
        /* Read binary file */
        int reading_binary = read_binary(inputFile1, input_file1name, img1);
        if(reading_binary != EXIT_NO_ERRORS)
        return reading_binary;
    }

    allocate_image_data(inputFile2, img2);

    if(img2->magic_number[1] == 53)
    {
        /* Read binary file */
        int reading_binary = read_binary(inputFile2, input_file2name, img2);
        if(reading_binary != EXIT_NO_ERRORS)
        return reading_binary;
    }

    if(img1->magic_number[1] != 53)
    {
        /* Read ascii file */
        int reading_ascii = read_ascii(inputFile1, input_file1name, img1);
        if(reading_ascii != EXIT_NO_ERRORS)
        return reading_ascii;
    }

    if(img2->magic_number[1] != 53)
    {
        /* Read ascii file */
        int reading_ascii = read_ascii(inputFile2, input_file2name, img2);
        if(reading_ascii != EXIT_NO_ERRORS)
        return reading_ascii;
    }
    
    /* Compare dimensions */
    if(img1->width == img2->width && img1->height == img2->height)
    {
        for(int i = 0; i < img1->nImageBytes; i++)
        { 
            /* Compare pixels */
            double pixel1 = (double)(img1->imageData[i]) / (double) img1->maxGray;

            double pixel2 = (double)(img2->imageData[i]) / (double) img2->maxGray;

            if(pixel1 != pixel2)
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