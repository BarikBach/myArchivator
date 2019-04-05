#include "tar.h"

int main(int argc, char const *argv[])
{
	printf("%d %s\n",argc, argv[0] );
	archive("tar.tar", "/Users/artem/Documents/Лабы весна 2019/Операционные системы/tes tTAR");
	printf("Archive OK\n");
	unarchive("/Users/artem/Documents/Лабы весна 2019/Операционные системы/lab1/tar.tar",
		   "/Users/artem/Documents/Лабы весна 2019/Операционные системы/lab1/");
	return 0;
	//
	// bool archive = false;
	// bool unarchive = false;
	// bool visible = false;
	// char *path_to_file = NULL;
	// char *path_to_archive = NULL;
	// for (int i = 1; i < argc; i++) {
	// 	if (!strcmp(argv[i]), "-c") {
	// 		archive = true;
	// 		continue;
	// 	}
	// 	if (!strcmp(argv[i]), "-v") {
	// 		visible = true;
	// 		continue;
	// 	}
	// 	if (!strcmp(argv[i]), "-x") {
	// 		unarchive = true;
	// 		continue;
	// 	}
	// 	if (path_to_archive == NULL) {
	// 		path_to_archive = argv[i];
	// 	} else {
	// 		path_to_file = argv[i];
	// 		break;
	// 	}
	// }
	// if (archive) {
	// 	archive(path_to_archive, path_to_file);
	// };
	// if (unarchive) {
	// 	unarchive(path_to_archive, path_to_file);
	// }
};
