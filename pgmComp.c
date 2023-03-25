#include <stdlib.h>
#include <stdio.h>

#include "pgmComp_func.h"

int main(int argc, char **argv)
{ 
	/* Call the function to read and write */
	int Comparing_PGM = pgmComp(argv[1], argv[2]);

	return Comparing_PGM;
} 