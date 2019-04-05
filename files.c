#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
char *get_new_path(const char *path, const char *name)
{
	char *newpath = NULL;
	int new_path_len = strlen(path)
			   + strlen(name) + 2;
	newpath = malloc(new_path_len * sizeof(char));
	if (newpath == NULL) {
		perror(NULL);
		return NULL;
	}
	strcpy(newpath, path);
	if (strlen(path) == 0) {
		strcpy((newpath + strlen(path)), name);
	} else {
		strcpy((newpath + strlen(path)), "/");
		strcpy((newpath + strlen(path) + 1), name);
	}
	return newpath;
}
int get_files(const char *dir, const char *path, int archive_file,
				int (*func)(int, const char*,
				const char *, const struct stat)) //33
{
	// Тупой обход каталога dir с вызовом для каждого файла функции func
	// func принимает значение типа int и имя каждого файла
	DIR *ptrcurrentDir = opendir(dir);
	if ((chdir(dir) != 0) || (ptrcurrentDir == NULL)) {
		perror(NULL);
		return -1;
	}
	const struct dirent *Dir;
	char *newpath = NULL;

	while ((Dir = readdir(ptrcurrentDir)) != NULL) {
		struct stat buff;
		stat(Dir->d_name, &buff);

		if ((strcmp(".", Dir->d_name) != 0) &&
		    (strcmp("..", Dir->d_name) != 0)) {
			newpath = get_new_path(path, Dir->d_name);
			if (func(archive_file, Dir->d_name, newpath, buff))
				return -1;
			if (S_ISDIR(buff.st_mode)) //Проваливаемся в директории
				if (get_files(Dir->d_name, newpath,
				    archive_file, func)){
					free(newpath);
					chdir("..");
					closedir(ptrcurrentDir);
					return -1;
				}

			free(newpath);
		}
	}
	chdir("..");
	closedir(ptrcurrentDir);
	return 0;
};

int create_dir(const char *name, const char mode[8])
{
	if (mkdir(name, byte_to_ll(mode, 8)) != 0) {
		if (errno != 17) { // directory exists
			perror(NULL);
			return -1;
		}
	}
	return 0;
}
int copy_data(int in, int out, int nbuffer, unsigned long long size)
{
	char buffer[nbuffer];
	unsigned long long i = 0ll;

	for (i = 0; i < (size < (unsigned long long) nbuffer ? 0 :
	     size - (long long)nbuffer); i += nbuffer) {
		int tmpcheck =  read(in, buffer, nbuffer);

		if (tmpcheck == -1)
			goto exit_copy;
		if (tmpcheck != nbuffer)
			goto exit_copy_demage;
		tmpcheck = write(out, buffer, nbuffer);
		if (tmpcheck == -1)
			goto exit_copy;
		if (tmpcheck != nbuffer)
			goto exit_copy_demage;
	}
	int tmpcheck = read(in, buffer, nbuffer);

	if (tmpcheck == -1)
		goto exit_copy;
	if (tmpcheck != nbuffer && tmpcheck != size % nbuffer)
		goto exit_copy_demage;
	tmpcheck = write(out, buffer, size % nbuffer);
	if (tmpcheck == -1)
		goto exit_copy;
	if (tmpcheck != size % nbuffer) {
		write(stderr, "Запись не удалась");
		return -1;
	}
	return size % nbuffer;
exit_copy:
	perror(NULL);
	return -1;
exit_copy_demage:
	write(stderr, "Файл поврежден");
	return -1;
}
