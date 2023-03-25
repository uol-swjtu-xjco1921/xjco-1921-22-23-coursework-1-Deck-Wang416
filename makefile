# set up our compiler settings
# we will be compiling using gcc
CC     = gcc
# setting some useful flags:
# std=c99 is the standard which you should be using
# -Wall means 'report all warnings' which is useful to help you write better code
# -Werror means 'make all warnings into errors' which means your code doesn't compile with warnings
# this is a good idea when code quality is important
# -g enables the use of GDB
CFLAGS = -std=c99 -Wall -Werror -g
# this is your list of executables which you want to compile with all
EXE    = pgmEcho pgma2b pgmb2a pgmComp pgmReduce

# we put 'all' as the first command as this will be run if you just enter 'make'
all: ${EXE}

# clean removes all object files - DO NOT UNDER ANY CIRCUMSTANCES ADD .c OR .h FILES
# rm is NOT REVERSIBLE.
clean: 
	rm -rf *.o ${EXE}

# this is a rule to define how .o files will be compiled
# it means we do not have to write a rule for each .o file
# pgmEcho.o will be compiled by this rule as:
# pgmEcho.o: pgmEcho.c
# gcc -c -std=c99 -g pgmEcho.c -o pgmEcho.o

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# for each executable, you need to tell the makefile the 'recipe' for your file
# at the moment, this is very simple as we are only using one .c file
# but as you refactor and add more .c and .h files
# these recipes will become more complex.
pgmEcho: pgmEcho.o pgmEcho_func.o pgma2b_func.o pgmb2a_func.o pgmComp_func.o pgmImage.o pgmReduce_func.o
	$(CC) $(CFLAGS) $^ -o $@

pgma2b: pgma2b.o pgma2b_func.o pgmb2a_func.o pgmComp_func.o pgmEcho_func.o pgmImage.o pgmReduce_func.o
	$(CC) $(CFLAGS) $^ -o $@

pgmb2a: pgmb2a.o pgmb2a_func.o pgma2b_func.o pgmComp_func.o pgmEcho_func.o pgmImage.o pgmReduce_func.o
	$(CC) $(CFLAGS) $^ -o $@

pgmComp: pgmComp.o pgmComp_func.o pgma2b_func.o pgmb2a_func.o pgmEcho_func.o pgmImage.o pgmReduce_func.o
	$(CC) $(CFLAGS) $^ -o $@

pgmReduce: pgmReduce.o pgmReduce_func.o pgma2b_func.o pgmb2a_func.o pgmComp_func.o pgmEcho_func.o pgmImage.o
	$(CC) $(CFLAGS) $^ -o $@