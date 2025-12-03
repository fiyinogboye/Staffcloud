# Simple Makefile with gcov

CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11 -g --coverage
LDFLAGS := --coverage

TARGET  := staffcloud

# All .c files in this folder
SRCS    := $(wildcard *.c)
OBJS    := $(SRCS:.c=.o)

# All .txt files in this folder (your 15 org files)
INPUTS  := $(wildcard *.txt)

.PHONY: all clean coverage

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run program on every .txt file, then generate gcov reports
coverage: $(TARGET)
	@echo "== Generating gcov reports =="
	gcov $(SRCS)

run: $(TARGET)
	@for f in $(INPUTS); do \
	  echo "== Running with $$f =="; \
	  ./$(TARGET) $$f || exit 1; \
	done

clean:
	rm -f $(TARGET) $(OBJS) *.gcno *.gcda *.gcov

