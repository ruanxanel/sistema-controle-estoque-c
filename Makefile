CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = $(wildcard src/*.c)
TARGET = build/estoque

all: $(TARGET)

$(TARGET): $(SRC) $(wildcard include/*.h)
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -rf build

.PHONY: all run clean