#include <stdio.h>
#include <stdlib.h>

#include "pgmb2a.h"
#include "handleFile.h"

// Convert binary files to ascii files
int pgmb2a(int argc, char* executable_name, char *input_filename, char *output_filename, FILE* inputFile, FILE *outputFile) 
{
    struct PGM_Image* img = new_img();

    // Check format and read binary file
    check_args(argc, executable_name);

    open_input_file(inputFile, input_filename);

    check_magic_number(inputFile, input_filename, img);

    read_comment_line(inputFile, img);

    read_image_header(inputFile, input_filename, img);

    allocate_image_data(inputFile, img);

    write_image_header(outputFile, output_filename, img);
    
    fread(img->imageData, sizeof(unsigned char), img->width * img-> height, inputFile);

    // Write ascii data
    writeMatrix(outputFile, output_filename, img);

    fclose(inputFile);
    fclose(outputFile);

    printf("CONVERTED");

    return EXIT_NO_ERRORS;
}
