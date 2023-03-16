#ifndef READING_FILE_H
#define READING_FILE_H

#include <stdio.h>
#include <stdlib.h>

#include "pgmImage.h"

#define EXIT_NO_ERRORS 0
#define EXIT_WRONG_ARG_COUNT 1
#define EXIT_BAD_FILE_NAME 2
#define EXIT_BAD_MAGIC_NUMBER 3
#define EXIT_BAD_COMMENT_LINE 4
#define EXIT_BAD_DIAMENSIONS 5
#define EXIT_BAD_MAX_GRAY_VALUE 6
#define EXIT_IMAGE_MALLOC_FAILED 7
#define EXIT_BAD_DATA 8
#define EXIT_OUTPUT_FAILED 9
#define EXIT_MISCELLANEOUS 100

#define MAGIC_NUMBER_ASCII_PGM 0x3250
#define MIN_IMAGE_DIMENSION 1
#define MAX_IMAGE_DIMENSION 65536
#define MAX_COMMENT_LINE_LENGTH 128

int handleFile(int argc, char* executable_name, char* input_filename, char* output_filename,
FILE *inputFile,FILE *outputFile);
int check_args(int argc, char* executable_name);
int open_input_file(FILE *inputFile, char *input_filename);
int check_magic_number(FILE *inputFile, char *input_filename, struct PGM_Image* img);
int read_comment_line(FILE *inputFile, struct PGM_Image* img);
int read_image_header(FILE *inputFile, char *input_filename, struct PGM_Image *img);
int allocate_image_data(FILE *inputFile, struct PGM_Image *img);
int sanity_check(FILE *inputFile, char *input_filename, struct PGM_Image* img);
int write_image_header(FILE* outputFile, char* output_filename, struct PGM_Image *img);
int writeMatrix(FILE* outputFile, char* output_filename, struct PGM_Image *img);

#endif