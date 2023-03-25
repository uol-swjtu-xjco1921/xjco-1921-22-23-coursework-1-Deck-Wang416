#include <stdio.h>
#include <stdlib.h>

#include "pgmb2a_func.h"

int main(int argc, char **argv)
{ 
	/* Call the function to read and write */
	int convert_to_ascii = pgmb2a(argv[1], argv[2], fopen(argv[1], "rb"),fopen(argv[2], "w"));
	return convert_to_ascii;
}