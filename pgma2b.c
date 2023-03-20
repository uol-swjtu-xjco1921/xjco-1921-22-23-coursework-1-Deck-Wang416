#include <stdio.h>
#include <stdlib.h>

#include "pgma2b.h"
#include "handleFile.h"

// Convert ascii files to binary files
int pgma2b(char *input_filename, char *output_filename, FILE* inputFile, FILE *outputFile) 
{
    struct PGM_Image* img = new_img();

    // Check format and read ASCII file
    int open_input_result = open_input_file(inputFile, input_filename);
    if(open_input_result != EXIT_NO_ERRORS)
    return open_input_result;

    img->magic_number[0] = getc(inputFile);
    img->magic_number[1] = getc(inputFile);
    unsigned short* magic_Number = (unsigned short *) img->magic_number;
    
    /* Only accept ascii corresponding number */
    if ((*magic_Number) != MAGIC_NUMBER_ASCII_PGM)
    {
        fclose(inputFile);
        printf("ERROR: Bad Magic Number (%s)\n", input_filename);
        return EXIT_BAD_MAGIC_NUMBER;
    }

    int comment_line_reading = read_comment_line(inputFile, img);
    if(comment_line_reading != EXIT_NO_ERRORS)
    return comment_line_reading;

    int image_header_reading = read_image_header(inputFile, input_filename, img);
    if(image_header_reading != EXIT_NO_ERRORS)
    return image_header_reading;

    int image_data_allocating = allocate_image_data(inputFile, img);  
    if(image_data_allocating != EXIT_NO_ERRORS)
    return image_data_allocating;

    /* Read ascii data */
    int reading_ascii = read_ascii(inputFile, input_filename, img);
    if(reading_ascii != EXIT_NO_ERRORS)
    return reading_ascii;

    write_image_header(outputFile, output_filename, img, 5);

    // Write binary data
    fwrite(img->imageData, sizeof(unsigned char), img->width * img-> height, outputFile);
    
    fclose(inputFile);

    printf("CONVERTED\n");

    return EXIT_NO_ERRORS;
}