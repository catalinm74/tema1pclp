CC = gcc
CFLAGS = -Wall -Wextra -Werrror -std=gnu11
LDLIBS = -lm

TARGET = battleships
SOURCES = battleships.c cleanup.c encryption_algorithm.c game.c printing.c statistics.c validation.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = cleanup.h encryption_algorithm.h game.h printing.h statistics.h types.h validation.h

.PHONY: all clean rebuild

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDLIBS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

rebuild: clean all
