CC = gcc
TARGET = hello
CFLAGS = -Wall -Wextra -fsanitize=address -Wno-implicit-function-declaration

.PHONY: all clean check
.SUFFIXES: .c .o



all: $(TARGET)


check:
		./checkpatch.pl --no-tree -f crc.c tar.c stack.c main.c

clean:
		rm -rf $(TARGET) *.o *.EXPERIMENTAL-checkpatch-fixes *.out

.c.o:
		$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): main.o crc.o tar.o bytes.o
			gcc -o $(TARGET) main.o crc.o tar.o bytes.o
