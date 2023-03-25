#include <stdlib.h>
#include <stdio.h>

#include "pgmReduce_func.h"
#include "pgmEcho_func.h"

/* Reduces the input PGM file by the given factor and creates a new output file */
int pgmReduce(char *input_filename, int factor, char *output_filename) 
{ 
    struct PGM_Image* img = new_img();

    FILE *inputFile = fopen(input_filename, "r");

    // Open and validate the input file
    open_input_file(inputFile, input_filename);

    // Read the magic number of the PGM format
    img->magic_number[0] = getc(inputFile);

    img->magic_number[1] = getc(inputFile);

    // Read the comment line and image header
    read_comment_line(inputFile, img);

    read_image_header(inputFile, input_filename, img);

    // Allocate memory for the image data
    allocate_image_data(inputFile, img);

    // Read the image data depending on the PGM format (binary or ASCII)
    if(img->magic_number[1] == 53)
    {
        // Read binary PGM file
        int reading_binary = read_binary(inputFile, input_filename, img);

        if(reading_binary != EXIT_NO_ERRORS)

            return reading_binary;
    }

    else
    {
        // Read ASCII PGM file
        int reading_ascii = read_ascii(inputFile, input_filename, img);

        if(reading_ascii != EXIT_NO_ERRORS)

            return reading_ascii;
    }

    FILE *outputFile = fopen(output_filename, "w");

    // long matrix_start_pos = ftell(outputFile);
    
    // Reserve the space of header in the output file
    for (int i = 0; i < 13; i++)
    {
        fprintf(outputFile, "\n");
    }
    
    int num = 0;

    int rowCount = 0;

    int total_colCount = 0;

    // Write the new reduced matrix to output file
    for (int i = 0; i < img->height; i++) 
    {
        for (int j = 0; j < img->width; j++) 
        {
            // Only write the pixel value if it meets the reduction factor criteria
            if (i % factor == 0 && j % factor == 0) 
            {
                if (img->magic_number[1] == 53)
                {
                    // Write binary data to the output file
                    unsigned char pixelValue = img->imageData[i * img->width + j];
                    fwrite(&pixelValue, sizeof(unsigned char), 1, outputFile);
                    num = 5;
                }

                else
                {
                    // Write ASCII data to the output file
                    fprintf(outputFile, "%d ", img->imageData[i * img->width + j]);
                    num = 2;
                }

                total_colCount++;
            } 
        }

        // Insert a newline character for each row that meets the reduction factor criteria
        if(i % factor == 0)
        {
            fprintf(outputFile, "\n");

            rowCount++;
        }
    }

    // Rewind the pointer to beginning in order to write the header
    rewind(outputFile);

    // Update the image dimensions based on the reduction factor
    img->height = rowCount;

    img->width = total_colCount/rowCount;

    // fseek(outputFile, matrix_start_pos, SEEK_SET);

    // Write the image header to the output file
    int image_header_writing = write_image_header(outputFile, output_filename, img, num);

    if(image_header_writing != EXIT_NO_ERRORS)

        return image_header_writing;

    fclose(inputFile);

    printf("REDUCED\n");

    return EXIT_NO_ERRORS;
}