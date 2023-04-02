#include <stdlib.h>
#include <stdio.h>

#include "pgmReduce_func.h"
#include "pgmEcho_func.h"

// Reduces input PGM file by given factor and creates a new output file
int pgmReduce(int argc, char* executable_name, char *input_filename, char* output_filename, char* factor_num) 
{ 
    // Check arguments
    if (argc == 1) 
    {
        printf("Usage: %s inputImage.pgm reduction_factor outputImage.pgm\n", executable_name);

        return EXIT_NO_ERRORS;
    }

    else if (argc != 4) 
    {
        printf("ERROR: Bad Argument Count\n");

        return EXIT_WRONG_ARG_COUNT;
    }

    int factor_check_result = check_factor_value(factor_num);

    if (factor_check_result != EXIT_NO_ERRORS)
    {
        return factor_check_result;
    }

    int factor= atoi(factor_num);

    // Open and validate the input file
    FILE* inputFile = fopen(input_filename, "r");

    int open_input_result = open_input_file(inputFile, input_filename);

    if(open_input_result != EXIT_NO_ERRORS)

        return open_input_result;
    
    struct PGM_Image* img = new_img();

    // Read the magic number of the PGM format
    int check_magic_result = check_magic_number(inputFile, input_filename, img);

    if(check_magic_result != EXIT_NO_ERRORS)

        return check_magic_result;

    // Read comment line
    int comment_line_reading = read_comment_line(inputFile, img);

    if(comment_line_reading != EXIT_NO_ERRORS)

        return comment_line_reading;
    
    // Read image header
    int image_header_reading = read_image_header(inputFile, input_filename, img);

    if(image_header_reading != EXIT_NO_ERRORS)

        return image_header_reading;

    // Allocate memory for the image data
    int image_data_allocating = allocate_image_data(inputFile, img);  

    if(image_data_allocating != EXIT_NO_ERRORS)

        return image_data_allocating;
    
    int num = 0;

    // Read the image data depending on the PGM format
    if(img->magic_number[1] == 53)
    {
        // Read binary PGM file
        num = 5;

        int reading_binary = read_binary(inputFile, input_filename, img);

        if(reading_binary != EXIT_NO_ERRORS)

            return reading_binary;
    }

    else
    {
        // Read ASCII PGM file
        num = 2;

        int reading_ascii = read_ascii(inputFile, input_filename, img);

        if(reading_ascii != EXIT_NO_ERRORS)

            return reading_ascii;
    }

    // Store original dimensions
    int original_height = img->height;

    int original_width = img->width;

    // Update the image dimensions based on the reduction factor
    img->height = img->height % factor == 0 ? img->height / factor : (img->height / factor) + 1;

    img->width = img->width % factor == 0 ? img->width / factor : (img->width / factor) + 1;

    // Write the image header to the output file
    FILE* outputFile = fopen(output_filename, "w");
    
    int image_header_writing = write_image_header(outputFile, output_filename, img, num);

    if(image_header_writing != EXIT_NO_ERRORS)

        return image_header_writing;

    // Restore original dimensions
    img->height = original_height;

    img->width = original_width;

    // Write the new reduced matrix to output file
    for (int i = 0; i < img->height; i++) 
    {
        for (int j = 0; j < img->width; j++) 
        {
            // Only write the pixel value if it meets the reduction factor criteria
            if (i % factor == 0 && j % factor == 0) 
            {
                if (num == 5)
                {
                    // Write binary data to the output file
                    unsigned char pixelValue = img->imageData[i * img->width + j];

                    fwrite(&pixelValue, sizeof(unsigned char), 1, outputFile);
                }

                else
                {
                    // Write ASCII data to the output file
                    fprintf(outputFile, "%d ", img->imageData[i * img->width + j]);
                }
            } 
        }

        // Insert a newline character for each row that meets the reduction factor criteria
        if(i % factor == 0)
        {
            fprintf(outputFile, "\n");
        }
    }
    
    // Free memory
    free_img(img);
    
    free(img);

    fclose(inputFile);

    fclose(outputFile);

    printf("REDUCED\n");

    return EXIT_NO_ERRORS;
}

// Check if the provided factor number is valid
int check_factor_value(char *factor_num)
{
    char *end_ptr;

    double value = strtod(factor_num, &end_ptr);

    if (end_ptr == factor_num || *end_ptr != '\0' || value == 0 || (int)value != value)
    {
        printf("ERROR: Miscellaneous (Invalid Factor)\n");

        return EXIT_MISCELLANEOUS;
    }

    return EXIT_NO_ERRORS;
}