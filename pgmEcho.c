#include <stdlib.h>
#include <stdio.h>

#include "pgmEcho_func.h"

int main(int argc, char **argv)
{ 
	/* Call the function to read and write */
	int handling_file = handleFile(argc, argv[0], argv[1], argv[2], fopen(argv[1], "r"),fopen(argv[2], "w"));

	return handling_file;
} 