CC = gcc
TARGET = hello
<<<<<<< HEAD
CFLAGS = -Wall -Wextra
=======
CFLAGS = -Wall -Wextra  -Wno-implicit-function-declaration
>>>>>>> f99c495fe81b198184fee63d28784bedfe4172ab

OFILES = main.o crc.o tar.o bytes.o files.o

.PHONY: all clean check
.SUFFIXES: .c .o



all: $(TARGET)


check:
<<<<<<< HEAD
=======
		./checkpatch.pl --no-tree -f main.с
>>>>>>> f99c495fe81b198184fee63d28784bedfe4172ab
		cppcheck main.c crc.c tar.c bytes.c files.c

clean:
		rm -rf $(TARGET) *.o *.EXPERIMENTAL-checkpatch-fixes *.out

.c.o:
		$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OFILES)
			gcc -o $(TARGET) $(OFILES)
run:
<<<<<<< HEAD
=======
	./hello -c tar.tar /Users/artem/Documents/Лабы\ весна\ 2019/Операционные\ системы/testTAR
	echo Archive OK
>>>>>>> f99c495fe81b198184fee63d28784bedfe4172ab
	./hello -c -v tar.tar /Users/artem/Documents/Лабы\ весна\ 2019/Операционные\ системы/testTAR
	echo Archive OK

	./hello -x -v /Users/artem/Documents/Лабы\ весна\ 2019/Операционные\ системы/lab1/tar.tar /Users/artem/Documents/Лабы\ весна\ 2019/Операционные\ системы/lab1/
