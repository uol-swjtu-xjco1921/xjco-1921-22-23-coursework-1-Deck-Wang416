#include <stdlib.h>
#include <stdio.h>

#include "handleFile.h"
#include "pgmImage.h"

int scanCount;

/* Read and write files */
int handleFile(int argc, char* executable_name, char* input_filename, char* output_filename,
FILE *inputFile, FILE *outputFile)
{
    struct PGM_Image* img = new_img();

    int check_args_result = check_args(argc, executable_name);
    if (check_args_result != EXIT_NO_ERRORS) 
    return check_args_result;

    int open_input_result = open_input_file(inputFile, input_filename);

    if(open_input_result != EXIT_NO_ERRORS)
    return open_input_result;

    int check_magic_result = check_magic_number(inputFile, input_filename, img);

    if(check_magic_result != EXIT_NO_ERRORS)
    return check_magic_result;
    
    int comment_line_reading = read_comment_line(inputFile, img);

    if(comment_line_reading != EXIT_NO_ERRORS)
    return comment_line_reading;

    int image_header_reading = read_image_header(inputFile, input_filename, img);

    if(image_header_reading != EXIT_NO_ERRORS)
    return image_header_reading;
    
    int image_data_allocating = allocate_image_data(inputFile, img);  

    if(image_data_allocating != EXIT_NO_ERRORS)
    return image_data_allocating;

    int checking_sanity = sanity_check(inputFile, input_filename, img);
    if(checking_sanity != EXIT_NO_ERRORS)
    return EXIT_BAD_DATA;

    int image_header_writing = write_image_header(outputFile, output_filename, img);
    if(image_header_writing != EXIT_NO_ERRORS)
    return EXIT_OUTPUT_FAILED;

    int matrixWriting = writeMatrix(outputFile, output_filename, img);
    if(matrixWriting != EXIT_NO_ERRORS)
    return EXIT_OUTPUT_FAILED;

    free_img(img);

    printf("ECHOED");	
    
	return EXIT_NO_ERRORS;
}
/* Check arguments */
int check_args(int argc, char* executable_name) 
{
    if (argc == 1) // Give user input prompt
    {
        printf("Usage: %s inputImage.pgm outputImage.pgm\n", executable_name);
        return EXIT_MISCELLANEOUS;
    }

    else if (argc != 3) 
    {
        printf("ERROR: Bad Arguments\n");
        return EXIT_WRONG_ARG_COUNT;
    }

    return EXIT_NO_ERRORS;

}
/* Open input file */
int open_input_file(FILE *inputFile, char *input_filename)
{
    if (inputFile == NULL)
    {
        printf("ERROR: Bad File Name (%s)\n", input_filename);
        return EXIT_BAD_FILE_NAME;
    }

    return EXIT_NO_ERRORS;
}
/* check magic number */
int check_magic_number(FILE *inputFile, char *input_filename, struct PGM_Image* img)
{
    img->magic_number[0] = getc(inputFile);
    img->magic_number[1] = getc(inputFile);
    unsigned short* magic_Number = (unsigned short *) img->magic_number;
    
    if ((*magic_Number) != MAGIC_NUMBER_ASCII_PGM)
    {
        fclose(inputFile);
        printf("ERROR: Bad Magic Number (%s)\n", input_filename);
        return EXIT_BAD_MAGIC_NUMBER;
    }

    return EXIT_NO_ERRORS;
}
// read commentline
int read_comment_line(FILE *inputFile, struct PGM_Image* img)
{
    scanCount = fscanf(inputFile, " ");
    char nextChar = fgetc(inputFile);

    if (nextChar == '#')
    {
        char *commentLine = NULL;

        commentLine = (char *) malloc(MAX_COMMENT_LINE_LENGTH);

		char *commentString = fgets(commentLine, MAX_COMMENT_LINE_LENGTH, inputFile);

        if (commentString == NULL)
        {
            free_img(img);
            fclose(inputFile);
            printf("ERROR: Bad Comment Line\n");
            return EXIT_BAD_COMMENT_LINE;
        }
    }
    
    else
    {
        ungetc(nextChar, inputFile);
    }

    return EXIT_NO_ERRORS;
}

/* read image header */
int read_image_header(FILE *inputFile, char *input_filename, struct PGM_Image *img)
{
    scanCount = fscanf(inputFile, " %u %u %u", &img->width, &img->height, &img->maxGray);

    if (scanCount != 3 || img->width < MIN_IMAGE_DIMENSION || img->width > MAX_IMAGE_DIMENSION ||
        img->height < MIN_IMAGE_DIMENSION || img->height > MAX_IMAGE_DIMENSION)
    {
        free_img(img);
        fclose(inputFile);
        printf("ERROR: Bad Dimensions (%s)\n", input_filename);
        return EXIT_BAD_DIAMENSIONS;
    }

    if (img->maxGray != 255)
    {
        free_img(img);
        fclose(inputFile);
        printf("ERROR: Bad Max Gray Value\n");
        return EXIT_BAD_MAX_GRAY_VALUE;
    }

    return EXIT_NO_ERRORS;
}
/* Allocate memory for image data */
int allocate_image_data(FILE *inputFile, struct PGM_Image *img) 
{  
    img->nImageBytes = img->width * img->height * sizeof(unsigned char);
    img->imageData = (unsigned char *) malloc(img->nImageBytes);

    if (img->imageData == NULL) 
    {
        free_img(img);
        fclose(inputFile);
        printf("ERROR: Image Malloc Failed \n");
        return EXIT_IMAGE_MALLOC_FAILED;
    }
    
    return EXIT_NO_ERRORS;
}
/* Sanity check */
int sanity_check(FILE *inputFile, char *input_filename, struct PGM_Image* img) 
{
    for (unsigned char *nextGrayValue = img->imageData; nextGrayValue < img->imageData + img->nImageBytes; nextGrayValue++) {
        int grayValue = -1;
        scanCount = fscanf(inputFile, " %u", &grayValue);

        if ((scanCount != 1) || (grayValue < 0) || (grayValue > 255)) 
        {
            free_img(img);

            fclose(inputFile);

            printf("ERROR: Bad Data (%s) \n", input_filename);

            return EXIT_BAD_DATA;
        }

        *nextGrayValue = (unsigned char) grayValue;
    }

    fclose(inputFile);

    return EXIT_NO_ERRORS;
}
/* Write file */
int write_image_header(FILE* outputFile, char* output_filename, struct PGM_Image *img) 
{
    if (outputFile == NULL) 
    {
        free_img(img);
       
        printf("ERROR: Output Failed (%s)\n", output_filename);
   
        return EXIT_OUTPUT_FAILED;
    }

	size_t nBytesWritten = fprintf(outputFile, "P%c\n%d %d\n%d\n", img->magic_number[1], img->width, img->height, img->maxGray);

	if (nBytesWritten < 0)
	{ 
		free_img(img);

		printf("ERROR: Output Failed (%s) \n", output_filename);	

		return EXIT_OUTPUT_FAILED;
	} 

    return EXIT_NO_ERRORS;
}

int writeMatrix(FILE* outputFile, char* output_filename, struct PGM_Image *img)
{
    for (unsigned char *nextGrayValue = img->imageData; nextGrayValue < img->imageData + img->nImageBytes; nextGrayValue++)
    { 
        int nextCol = (nextGrayValue - img->imageData + 1) % img->width;
        
        size_t nBytesWritten = fprintf(outputFile, "%d%c", *nextGrayValue, (nextCol ? ' ' : '\n') );

        if (nBytesWritten < 0)
        { 
            free_img(img);

            printf("ERROR: Output Failed (%s)\n", output_filename);	

            return EXIT_OUTPUT_FAILED;
        } 
    } 
    
    return EXIT_NO_ERRORS;
}
