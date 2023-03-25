#include <stdio.h>
#include <stdlib.h>

#include "pgmb2a_func.h"
#include "pgmEcho_func.h"

int pgmb2a_main(int argc, char **argv)
{ 
	/* Call the function to read and write */
	int convert_to_ascii = pgmb2a(argv[1], argv[2], fopen(argv[1], "rb"),fopen(argv[2], "w"));
	return convert_to_ascii;
} 

// Convert binary files to ascii files
int pgmb2a(char *input_filename, char *output_filename, FILE* inputFile, FILE *outputFile) 
{
    struct PGM_Image* img = new_img();

    // Open input file 
    int open_input_result = open_input_file(inputFile, input_filename);
    if(open_input_result != EXIT_NO_ERRORS)
        return open_input_result;

    // Check magic number
    img->magic_number[0] = getc(inputFile);
    img->magic_number[1] = getc(inputFile);
    unsigned short* magic_Number = (unsigned short *) img->magic_number;
    
    // Only accept binary PGM magic number
    if ((*magic_Number) != MAGIC_NUMBER_BINARY_PGM)
    {
        fclose(inputFile);
        printf("ERROR: Bad Magic Number (%s)\n", input_filename);
        return EXIT_BAD_MAGIC_NUMBER;
    }

    // Read comment line (if present) from the file
    int comment_line_reading = read_comment_line(inputFile, img);
    if(comment_line_reading != EXIT_NO_ERRORS)
        return comment_line_reading;

    // Read image header from the file
    int image_header_reading = read_image_header(inputFile, input_filename, img);
    if(image_header_reading != EXIT_NO_ERRORS)
        return image_header_reading;

    // Allocate memory for the image data
    int image_data_allocating = allocate_image_data(inputFile, img);
    if(image_data_allocating != EXIT_NO_ERRORS)
        return image_data_allocating;

    // Write image header for the ASCII PGM output file
    int image_header_writing = write_image_header(outputFile, output_filename, img, 2);
    if(image_header_writing != EXIT_NO_ERRORS)
        return image_header_writing;

    // Read binary PGM data from the input file
    int reading_binary = read_binary(inputFile, input_filename, img);
    if(reading_binary != EXIT_NO_ERRORS)
        return reading_binary;
    
    // Write ASCII data to the output file
    int matrixWriting = writeMatrix(outputFile, output_filename, img);
    if(matrixWriting != EXIT_NO_ERRORS)
        return matrixWriting;

    fclose(inputFile);

    printf("CONVERTED\n");

    return EXIT_NO_ERRORS;
}