#include <stdlib.h>
#include <stdio.h>

#include "pgmReduce_func.h"

int main(int argc, char **argv) 
{
    return pgmReduce(argc, argv[0], argv[1], argv[3], argv[2]);
}