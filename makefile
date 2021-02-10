LINUX_GCC=gcc
LINUX_CFLAGS=-Wall -pthread
# LINUX_CFLAGS_END=-lrt
LINUX_CFLAGS_END=

.PHONY = all run clean help

SRCS := $(wildcard *.c)
OBJ := $(SRCS:%.c=%.o)

all: main.out

main.out: ${OBJ}
	@echo "Linking files"
	$(LINUX_GCC) $(LINUX_CFLAGS) -o main.out ${OBJ} $(LINUX_CFLAGS_END)


run: main.out
	@echo "Running from bin/main.out"
	@./main.out ./ c h

clean:
	@echo "Cleaning up"
	rm *.o
	rm *.out

main.o: main.c common.h
	$(LINUX_GCC) $(LINUX_CFLAGS) -c main.c $(LINUX_CFLAGS_END)
	
%.o: %.c %.h common.h
	$(LINUX_GCC) $(LINUX_CFLAGS) -c $< $(LINUX_CFLAGS_END)

help:
	@echo "Makefile for C Project"
	@echo "Course: System Programming and Concurent Programming"
	@echo "Computing 3 sem, Faculty of Computing and Telecomunications, Poznań Univeristy of Technology Winter 2020/2021."
	@echo "Author: Marcin Kasznia"
	@echo
	@echo "Make options:"
	@echo "  all:    links all .o files into bin/main.out executable file"
	@echo "  run:    runs the ./main.out executable"
	@echo "  clean:  cleans project files (.o and .out)"