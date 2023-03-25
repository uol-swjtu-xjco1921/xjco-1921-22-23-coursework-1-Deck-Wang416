#include <stdlib.h>
#include <stdio.h>

#include "pgmReduce_func.h"

int main(int argc, char **argv) 
{
    // Call pgmReduce with command line arguments
    int reducing_pgm = pgmReduce(argv[1], atoi(argv[2]), argv[3]);

    return reducing_pgm;
}