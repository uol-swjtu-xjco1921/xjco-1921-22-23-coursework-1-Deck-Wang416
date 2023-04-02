#ifndef PGMREDUCE_H
#define PGMREDUCE_H

#include "pgmEcho_func.h"

int pgmReduce(int argc, char* executable_name, char *input_filename, char* output_filename, char* factor_num);

int check_factor_value(char *factor_num);

#endif