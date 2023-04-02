#include <stdlib.h>
#include <stdio.h>

#include "pgmEcho_func.h"
#include "pgmImage.h"

int scanCount;

/* Read and write files */
int handleFile(int argc, char* executable_name, char* input_filename, char* output_filename)
{
    // Check arguments
    int check_args_result = check_args(argc, executable_name);
    
    if (check_args_result == EXIT_MISCELLANEOUS) 

        return EXIT_NO_ERRORS;

    else if (check_args_result != EXIT_NO_ERRORS) 

        return check_args_result;

    // Open input file
    FILE* inputFile = fopen(input_filename, "r");

    int open_input_result = open_input_file(inputFile, input_filename);

    if(open_input_result != EXIT_NO_ERRORS)

        return open_input_result;
    
    struct PGM_Image* img = new_img();

    // Check magic number
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
    
    // Allocate memory for image data
    int image_data_allocating = allocate_image_data(inputFile, img);  

    if(image_data_allocating != EXIT_NO_ERRORS)

        return image_data_allocating;

    int num = 0;

    // Check if the file is binary format
    if(img->magic_number[1] == 53)
    {
        // Read binary file
        int reading_binary = read_binary(inputFile, input_filename, img);

        num = 5;

        if(reading_binary != EXIT_NO_ERRORS)

            return reading_binary;
    }

    // Check if the file is ascii format
    if(img->magic_number[1] != 53)
    {
        // Read ascii file
        int reading_ascii = read_ascii(inputFile, input_filename, img);

        num = 2;

        if(reading_ascii != EXIT_NO_ERRORS)

            return reading_ascii;
    }

    // Write image header
    FILE* outputFile = fopen(output_filename, "w");
    
    int image_header_writing = write_image_header(outputFile, output_filename, img, num);

    if(image_header_writing != EXIT_NO_ERRORS)

        return image_header_writing;

    // Write binary data
    if(img->magic_number[1] == 53)
    {
        fwrite(img->imageData, sizeof(unsigned char), img->width * img-> height, outputFile);
    }
    
    // Write ascii data matrix
    if(img->magic_number[1] != 53)
    {
        int matrixWriting = writeMatrix(outputFile, output_filename, img);

        if(matrixWriting != EXIT_NO_ERRORS)

            return matrixWriting;
    }

    // Free memory
    free_img(img);
    
    free(img);

    fclose(inputFile);

    fclose(outputFile);

    printf("ECHOED\n"); 
    
    return EXIT_NO_ERRORS;
}

// Check arguments
int check_args(int argc, char* executable_name) 
{
    // Give user input prompt
    if (argc == 1) 
    {
        printf("Usage: %s inputImage.pgm outputImage.pgm\n", executable_name);

        return EXIT_MISCELLANEOUS;
    }

    // Check if there is incorrect number of arguments
    else if (argc != 3) 
    {
        printf("ERROR: Bad Argument Count\n");

        return EXIT_WRONG_ARG_COUNT;
    }

    return EXIT_NO_ERRORS;
}

// Open input file
int open_input_file(FILE *inputFile, char *input_filename)
{
    // Check if input file is not valid
    if (inputFile == NULL) 
    {
        printf("ERROR: Bad File Name (%s)\n", input_filename);

        return EXIT_BAD_FILE_NAME;
    }

    return EXIT_NO_ERRORS;
}

// Check magic number
int check_magic_number(FILE *inputFile, char *input_filename, struct PGM_Image* img)
{
    img->magic_number[0] = getc(inputFile);

    img->magic_number[1] = getc(inputFile);

    unsigned short* magic_Number = (unsigned short *) img->magic_number;

    // Check whether the magic number is reasonable
    if ((*magic_Number) != MAGIC_NUMBER_ASCII_PGM && (*magic_Number) != MAGIC_NUMBER_BINARY_PGM)
    {
        free(img);

        fclose(inputFile);

        printf("ERROR: Bad Magic Number (%s)\n", input_filename);

        return EXIT_BAD_MAGIC_NUMBER;
    }

    return EXIT_NO_ERRORS;
}

// Read comment line
int read_comment_line(FILE *inputFile, struct PGM_Image* img)
{
    scanCount = fscanf(inputFile, " ");

    char nextChar = fgetc(inputFile);

    // Check if there is a comment line
    if (nextChar == '#') 
    {
        char *commentLine = NULL;

        // Allocate memory for comment line
        commentLine = (char *) malloc(MAX_COMMENT_LINE_LENGTH);

        img->commentLine = commentLine;

        // Read comment line
        char *commentString = fgets(commentLine, MAX_COMMENT_LINE_LENGTH, inputFile);

        // Check if reading the comment line was successful
        if (commentString == NULL)
        {
            free_img(img);

            free(img);

            fclose(inputFile);

            printf("ERROR: Bad Comment Line\n");

            return EXIT_BAD_COMMENT_LINE;
        }
    }

    // If there is no comment line, put the character back
    else 
    {
        ungetc(nextChar, inputFile);
    }

    return EXIT_NO_ERRORS;
}

// Read image header
int read_image_header(FILE *inputFile, char *input_filename, struct PGM_Image *img)
{
    // Read image dimensions and max gray value
    scanCount = fscanf(inputFile, " %u %u %u", &img->width, &img->height, &img->maxGray);

    // Check if dimensions are within acceptable limits
    if (scanCount != 3 || img->width < MIN_IMAGE_DIMENSION || img->width > MAX_IMAGE_DIMENSION ||
        img->height < MIN_IMAGE_DIMENSION || img->height > MAX_IMAGE_DIMENSION)
    {
        free_img(img);

        free(img);

        fclose(inputFile);
        
        printf("ERROR: Bad Dimensions (%s)\n", input_filename);

        return EXIT_BAD_DIAMENSIONS;
    }

    // Check if max gray value is valid
    if (img->maxGray != 255)
    {
        free_img(img);

        free(img);

        fclose(inputFile);

        printf("ERROR: Bad Max Gray Value\n");

        return EXIT_BAD_MAX_GRAY_VALUE;
    }

    return EXIT_NO_ERRORS;
}

// Allocate memory for image data
int allocate_image_data(FILE *inputFile, struct PGM_Image *img) 
{  
    // Calculate the number of bytes needed for image data
    img->nImageBytes = img->width * img->height * sizeof(unsigned char);

    // Allocate memory for image data
    img->imageData = (unsigned char *) malloc(img->nImageBytes);

    // Check if memory allocation was successful
    if (img->imageData == NULL) 
    {
        free_img(img);

        free(img);

        fclose(inputFile);

        printf("ERROR: Image Malloc Failed \n");

        return EXIT_IMAGE_MALLOC_FAILED;
    }
    
    return EXIT_NO_ERRORS;
}

// Read data from ascii files
int read_ascii(FILE *inputFile, char *input_filename, struct PGM_Image* img) 
{
    // Read image data from the input file
    for (unsigned char *nextGrayValue = img->imageData; nextGrayValue < img->imageData + img->nImageBytes; nextGrayValue++) 
    {
        int grayValue = -1;
        
        scanCount = fscanf(inputFile, " %u", &grayValue);

        // Check if the data read is valid
        if ((scanCount != 1) || (grayValue < 0) || (grayValue > 255) ) 
        {
            free_img(img);

            free(img);

            fclose(inputFile);

            printf("ERROR: Bad Data (%s) \n", input_filename);

            return EXIT_BAD_DATA;
        }

        *nextGrayValue = (unsigned char) grayValue;
    }

    // Attempt to read an extra data value
    int extraGrayValue = -1;

    int extraScanCount = fscanf(inputFile, " %u", &extraGrayValue);

    // If an extra data value is successfully read, it implies incorrect width and height
    if (extraScanCount == 1) 
    {
        free_img(img);

        free(img);

        fclose(inputFile);

        printf("ERROR: Bad Data (%s) \n", input_filename);

        return EXIT_BAD_DATA;
    }

    return EXIT_NO_ERRORS;
}

// Read data from binary files
int read_binary(FILE *inputFile, char *input_filename, struct PGM_Image* img)
{
    // Read and discard line break from input file
    unsigned char first_byte;

    fread(&first_byte, 1, 1, inputFile);

    // Read the image data from input file
    size_t bytes_read = fread(img->imageData, 1, img->nImageBytes, inputFile);

    // Check if the data read is valid
    if (bytes_read != img->nImageBytes)
    {
        free_img(img);

        free(img);

        fclose(inputFile);

        printf("ERROR: Bad Data (%s) \n", input_filename);

        return EXIT_BAD_DATA;
    }

    // Attempt to read an extra data value
    unsigned char extraGrayValue;

    size_t extra_bytes_read = fread(&extraGrayValue, 1, 1, inputFile);

    // If an extra data value is successfully read, it implies incorrect width and height
    if (extra_bytes_read == 1)
    {
        free_img(img);

        free(img);

        fclose(inputFile);

        printf("ERROR: Bad Data (%s) \n", input_filename);

        return EXIT_BAD_DATA;
    }

    return EXIT_NO_ERRORS;
}


// Write image header to the output file
int write_image_header(FILE* outputFile, char* output_filename, struct PGM_Image *img, char num) 
{
    if (outputFile == NULL) 
    {
        free_img(img);

        free(img);
       
        printf("ERROR: Output Failed (%s)\n", output_filename);
   
        return EXIT_OUTPUT_FAILED;
    }
    
    // Write image header to the output file
    size_t nBytesWritten = fprintf(outputFile, "P%d\n%d %d\n%d\n", num, img->width, img->height, img->maxGray);

    if (nBytesWritten < 0)
    { 
        free_img(img);

        free(img);

        fclose(outputFile);

        printf("ERROR: Output Failed (%s) \n", output_filename);	

        return EXIT_OUTPUT_FAILED;
    } 

    return EXIT_NO_ERRORS;
}

// Write image data matrix to output file
int writeMatrix(FILE* outputFile, char* output_filename, struct PGM_Image *img)
{
    // Iterate through image data and write each gray value to output file
    for (unsigned char *nextGrayValue = img->imageData; nextGrayValue < img->imageData + img->nImageBytes; nextGrayValue++)
    { 
        // Determine column of the current gray value
        int nextCol = (nextGrayValue - img->imageData + 1) % img->width;
        
        // Write gray value followed by a space or newline, depending on the column
        size_t nBytesWritten = fprintf(outputFile, "%d%c", *nextGrayValue, (nextCol ? ' ' : '\n') );

        // Check if the writing operation was successful
        if (nBytesWritten < 0)
        { 
            free_img(img);

            free(img);

            fclose(outputFile);

            printf("ERROR: Output Failed (%s)\n", output_filename);	

            return EXIT_OUTPUT_FAILED;
        } 
    }   
    return EXIT_NO_ERRORS;
}