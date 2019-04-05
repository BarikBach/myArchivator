CC = gcc
TARGET = hello
CFLAGS = -w
# -Wall -Wextra -fsanitize=address
.PHONY: all clean check
.SUFFIXES: .c .o



all: $(TARGET)


check:
		./checkpatch.pl --no-tree -f crc.c tar.c stack.c main.c

clean:
		rm -rf $(TARGET) *.o *.EXPERIMENTAL-checkpatch-fixes *.out

.c.o:
		$(CC) $(CFLAGS)  -o $@ $<

$(TARGET): main.o crc.o tar.o stack.o
			gcc -o $(TARGET) main.o crc.o tar.o stack.o
