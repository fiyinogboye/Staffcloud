

CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11 -g --coverage
LDFLAGS := --coverage

TARGET  := staffcloud


SRCS    := $(wildcard *.c)
OBJS    := $(SRCS:.c=.o)


INPUTS  := $(wildcard *.txt)

.PHONY: all clean coverage

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


coverage: $(TARGET)
	@for f in $(INPUTS); do \
	  echo "== Running with $$f =="; \
	  ./$(TARGET) $$f || exit 1; \
	done
	@echo "== Generating gcov reports =="
	gcov $(SRCS)

clean:
	rm -f $(TARGET) $(OBJS) *.gcno *.gcda *.gcov

