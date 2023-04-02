#include <stdio.h>
#include <stdlib.h>

#include "pgma2b_func.h"
#include "pgmEcho_func.h"

// Convert ascii files to binary files
int pgma2b(int argc, char* executable_name, char *input_filename, char *output_filename) 
{
    // Check arguments
    int check_args_result = check_args(argc, executable_name);
    
    if (check_args_result == EXIT_MISCELLANEOUS) 

        return EXIT_NO_ERRORS;

    else if (check_args_result != EXIT_NO_ERRORS) 

        return check_args_result;

    // Open input file 
    FILE* inputFile = fopen(input_filename, "rb");

    int open_input_result = open_input_file(inputFile, input_filename);

    if(open_input_result != EXIT_NO_ERRORS)

        return open_input_result;
    
    struct PGM_Image* img = new_img();
        
    // Check magic number
    img->magic_number[0] = getc(inputFile);

    img->magic_number[1] = getc(inputFile);

    unsigned short* magic_Number = (unsigned short *) img->magic_number;
    
    // Only accept ASCII PGM magic number
    if ((*magic_Number) != MAGIC_NUMBER_ASCII_PGM)
    {
        free(img);
        
        fclose(inputFile);

        printf("ERROR: Bad Magic Number (%s)\n", input_filename);

        return EXIT_BAD_MAGIC_NUMBER;
    }

    // Read comment line from input file
    int comment_line_reading = read_comment_line(inputFile, img);

    if(comment_line_reading != EXIT_NO_ERRORS)

        return comment_line_reading;

    // Read image header from input file
    int image_header_reading = read_image_header(inputFile, input_filename, img);

    if(image_header_reading != EXIT_NO_ERRORS)

        return image_header_reading;

    // Allocate memory for the image data
    int image_data_allocating = allocate_image_data(inputFile, img);  

    if(image_data_allocating != EXIT_NO_ERRORS)

        return image_data_allocating;

    // Read ASCII PGM data from input file
    int reading_ascii = read_ascii(inputFile, input_filename, img);

    if(reading_ascii != EXIT_NO_ERRORS)
    
        return reading_ascii;
    
    // Write image header for the output file
    FILE* outputFile = fopen(output_filename, "w");

    write_image_header(outputFile, output_filename, img, 5);

    // Write binary data to the output file
    fwrite(img->imageData, sizeof(unsigned char), img->width * img-> height, outputFile);

    // Free memory
    free_img(img);

    free(img);
    
    fclose(inputFile);

    fclose(outputFile);

    printf("CONVERTED\n");

    return EXIT_NO_ERRORS;
}