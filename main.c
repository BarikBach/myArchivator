#include "tar.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
int main(int argc, char const *argv[])
{
	bool archive = false;
	bool unarchive = false;
	bool visible = false;
	char *path_to_file = NULL;
	char *path_to_archive = NULL;
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i]), "-c") {
			archive = true;
			continue;
		}
		if (!strcmp(argv[i]), "-v") {
			visible = true;
			continue;
		}
		if (!strcmp(argv[i]), "-x") {
			unarchive = true;
			continue;
		}
		if (path_to_archive == NULL) {
			path_to_archive = argv[i];
		} else {
			path_to_file = argv[i];
			break;
		}
	}
	if (archive) {
		archive(path_to_archive, path_to_file);
	};
	if (unarchive) {
		unarchive(path_to_archive, path_to_file);
	}
};
