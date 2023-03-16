#include <stdio.h>
#include <stdlib.h>

#include "pgma2b.h"
#include "handleFile.h"

// Convert ascii files to binary files
int pgma2b(int argc, char* executable_name, char *input_filename, char *output_filename, FILE* inputFile, FILE *outputFile) 
{
    struct PGM_Image* img = new_img();

    // Check format and read ASCII file
    check_args(argc, executable_name);

    open_input_file(inputFile, input_filename);

    check_magic_number(inputFile, input_filename, img);

    read_comment_line(inputFile, img);

    read_image_header(inputFile, input_filename, img);

    allocate_image_data(inputFile, img);

    sanity_check(inputFile, input_filename, img);

    write_image_header(outputFile, output_filename, img);

    // Write binary data
    fwrite(img->imageData,sizeof(unsigned char), img->width * img-> height, outputFile);
    
    fclose(inputFile);
    fclose(outputFile);

    printf("CONVERTED");

    return EXIT_NO_ERRORS;
}