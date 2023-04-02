#include <stdlib.h>
#include <stdio.h>

#include "pgmEcho_func.h"

int main(int argc, char **argv)
{ 
	return handleFile(argc, argv[0], argv[1], argv[2]);
} 