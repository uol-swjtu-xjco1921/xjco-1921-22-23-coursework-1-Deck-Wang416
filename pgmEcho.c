#include <stdlib.h>
#include <stdio.h>

#include "handleFile.h"
#include "pgmImage.h"

int main(int argc, char **argv)
{ 
	/* Call the function to read and write */
	switch(handleFile(argc, argv[0], argv[1], argv[2], fopen(argv[1], "r"),fopen(argv[2], "w")))
	{
		case EXIT_WRONG_ARG_COUNT:
			return EXIT_WRONG_ARG_COUNT;
		case EXIT_BAD_FILE_NAME:
			return EXIT_BAD_FILE_NAME;
		case EXIT_BAD_MAGIC_NUMBER:
			return EXIT_BAD_MAGIC_NUMBER;
		case EXIT_BAD_COMMENT_LINE:
			return EXIT_BAD_COMMENT_LINE;
		case EXIT_BAD_DIAMENSIONS:
			return EXIT_BAD_DIAMENSIONS;
		case EXIT_BAD_MAX_GRAY_VALUE:
			return EXIT_BAD_MAX_GRAY_VALUE;
		case EXIT_IMAGE_MALLOC_FAILED:
			return EXIT_IMAGE_MALLOC_FAILED;
		case EXIT_BAD_DATA:
			return EXIT_BAD_DATA;
		case EXIT_OUTPUT_FAILED:
			return EXIT_OUTPUT_FAILED;
		default:
			return EXIT_NO_ERRORS;
	}
} 