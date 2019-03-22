all:
	gcc  main.c tar.c -o lab1 -std=c99
	./lab1 /Users/artem/Documents/Лабы\ весна\ 2019/Операционные\ системы/lab1
debug:
	gcc  main.c tar.c -o lab1 -Wall -Wextra -fsanitize=address -std=c99
	cppcheck main.c
	./checkpatch.pl --no-tree -f main.c
	./lab1 /Users/artem/Documents/Лабы\ весна\ 2019/Операционные\ системы/lab1
