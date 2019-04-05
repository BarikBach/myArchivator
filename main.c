#include "tar.h"
#include <stdbool.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{

	// printf("%d %s\n",argc, argv[0] );
	// archive("tar.tar", "/Users/artem/Documents/Лабы весна 2019/Операционные системы/testTAR");
	// printf("Archive OK\n");
	// unarchive("/Users/artem/Documents/Лабы весна 2019/Операционные системы/lab1/tar.tar",
	// 	   "/Users/artem/Documents/Лабы весна 2019/Операционные системы/lab1/");
	// return 0;
	//
	bool flag_archive = false;
	bool flag_unarchive = false;
	const char *path_to_file = NULL;
	const char *path_to_archive = NULL;
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-c")) {
			flag_archive = true;
			continue;
		}
		if (!strcmp(argv[i], "-v")) {
			visible_progress = true;
			continue;
		}
		if (!strcmp(argv[i], "-x")) {
			flag_unarchive = true;
			continue;
		}
		if (path_to_archive == NULL) {
			path_to_archive = argv[i];
		} else {
			path_to_file = argv[i];
			break;
		}
	}
	if (flag_archive) {
		archive(path_to_archive, path_to_file);
	};
	if (flag_unarchive) {
		unarchive(path_to_archive, path_to_file);
	}
};
