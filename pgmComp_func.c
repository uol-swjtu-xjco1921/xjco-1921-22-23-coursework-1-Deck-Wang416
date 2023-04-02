#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "pgmComp_func.h"
#include "pgmEcho_func.h"
#include "pgma2b_func.h"
#include "pgmb2a_func.h"

// Compare whether two files have the same logical content
int pgmComp(int argc, char* executable_name, char *input_file1name, char *input_file2name)
{
    // Check arguments
    if (argc == 1) 
    {
        printf("Usage: %s inputImage.pgm inputImage.pgm\n", executable_name);

        return EXIT_NO_ERRORS;
    }

    else if (argc != 3)
    {
        printf("ERROR: Bad Argument Count\n");

        return EXIT_WRONG_ARG_COUNT;
    }

    // Open input files
    FILE* inputFile1 = fopen(input_file1name, "r");

    int open_input_result1 = open_input_file(inputFile1, input_file1name);

    if(open_input_result1 != EXIT_NO_ERRORS)

        return open_input_result1;
    
    FILE* inputFile2 = fopen(input_file2name, "r");

    int open_input_result2 = open_input_file(inputFile2, input_file2name);

    if(open_input_result2 != EXIT_NO_ERRORS)

        return open_input_result2;
    
    struct PGM_Image* img1 = new_img();

    struct PGM_Image* img2 = new_img();

    // Read magic number from both files respectively
    int check_magic_result1 = check_magic_number(inputFile1, input_file1name, img1);

    if(check_magic_result1 != EXIT_NO_ERRORS)

        return check_magic_result1;
    
    int check_magic_result2 = check_magic_number(inputFile2, input_file2name, img2);

    if(check_magic_result2 != EXIT_NO_ERRORS)

        return check_magic_result2;

    // Read comment lines from both files respectively
    int comment_line_reading1 = read_comment_line(inputFile1, img1);

    if(comment_line_reading1 != EXIT_NO_ERRORS)

        return comment_line_reading1;

    int comment_line_reading2 = read_comment_line(inputFile2, img2);

    if(comment_line_reading2 != EXIT_NO_ERRORS)

        return comment_line_reading2;

    // Read image headers from both files respectively
    int image_header_reading1 = read_image_header(inputFile1, input_file1name, img1);

    if(image_header_reading1 != EXIT_NO_ERRORS)

        return image_header_reading1;

    int image_header_reading2 = read_image_header(inputFile2, input_file2name, img2);

    if(image_header_reading2 != EXIT_NO_ERRORS)

        return image_header_reading2;

    // Allocate memory respectively for image data
    int image_data_allocating1 = allocate_image_data(inputFile1, img1);  

    if(image_data_allocating1 != EXIT_NO_ERRORS)

        return image_data_allocating1;

    int image_data_allocating2 = allocate_image_data(inputFile2, img2);  

    if(image_data_allocating2 != EXIT_NO_ERRORS)

        return image_data_allocating2;

    // Read binary data if the magic number indicates Binary PGM files
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

    // Read ASCII data if the magic number indicates ASCII PGM files
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
    bool are_different_pgm = img1->width != img2->width || img1->height != img2->height;
    
    if (!are_different_pgm)
    {
        for (int i = 0; i < img1->nImageBytes; i++)
        {
            // Compare pixels by calculating the normalized gray values
            double pixel1 = (double)(img1->imageData[i]) / (double)img1->maxGray;

            double pixel2 = (double)(img2->imageData[i]) / (double)img2->maxGray;

            if (pixel1 != pixel2)
            {
                are_different_pgm = true;

                break;
            }
        }
    }

    // Free memory
    free_img(img1);

    free_img(img2);

    free(img1);

    free(img2);

    fclose(inputFile1);

    fclose(inputFile2);

    printf(are_different_pgm ? "DIFFERENT\n" : "IDENTICAL\n");

    return EXIT_NO_ERRORS;
}