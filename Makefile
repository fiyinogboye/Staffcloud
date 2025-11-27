# Makefile for Team Assignment - builds the staffcloud program
# AUTHOR: Fiyin Ogboye
# DATE: Nov 2025
# VERSION: 1.0

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC_TEST = src/hierarchy.c src/main2.c
OBJ_TEST = $(SRC_TEST:.c=.o)

# the target for now
all: fiyin_test

# Rule to link the test program
fiyin_test: $(OBJ_TEST)
	@echo "Linking fiyin_test..."
	$(CC) $(OBJ_TEST) -o fiyin_test
	@echo "Successfully compiled! Run with: ./fiyin_test"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o fiyin_test