
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = staffcloud
SOURCES = main.c search_analytics.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run
