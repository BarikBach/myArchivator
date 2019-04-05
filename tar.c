#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <utime.h>

#include "bytes.h"
#include "tar.h"


int archive(const char *archive_name, const char *path)
{
	int out = open(archive_name, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	const char *path1 = (char *) calloc(1, sizeof(char));

	get_files(path, path1, out, write_archive);
	if (putnull(out, 1024) != 1024)
		return -1;
	if (close(out) != 0)
		return -1;
	return 0;
};
char * get_new_path (const char *path, const char* name)
{
	char * newpath = NULL;
	int new_path_len = strlen(path)
			   + strlen(name) + 2;
	newpath = malloc(new_path_len * sizeof(char));
	if (newpath == NULL)
		return NULL;
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

	if ((chdir(dir) != 0) || (ptrcurrentDir == NULL))
		return -1;
	const struct dirent *Dir;
	char *newpath = NULL;

	while ((Dir = readdir(ptrcurrentDir)) != NULL) {
		struct stat buff;
		stat(Dir->d_name, &buff);
		if ((strcmp(".", Dir->d_name) != 0) &&
		    (strcmp("..", Dir->d_name) != 0)) {
			newpath = get_new_path(path, Dir->d_name);

			printf("%s\n", newpath);
			func(archive_file, Dir->d_name, newpath, buff);
			if (S_ISDIR(buff.st_mode)) //Проваливаемся в директории
				get_files(Dir->d_name, newpath, archive_file,
					  func);
			free(newpath);
		}
	}
	chdir("..");
	closedir(ptrcurrentDir);
	return 0;
};




int write_archive(int archive, const char *file_name, const char *path,
				  const struct stat file_stat)              //34
{
	if (write_header(archive, stat_to_header(path, file_stat)))
		printf("Что-то пошло не так\n");
	if (S_ISDIR(file_stat.st_mode))
		return 0;
	int in = open(file_name, O_RDONLY);
	if (in == -1) {
		perror(NULL);
		return -1;
	}
	char buffer[512];
	int nread;
	int error = 0;
	// Копирование информации
	while ((nread = read(in, buffer, 512)) != 0) {
		if (nread == -1)
			goto exit;
		if (nread != 512) {
			for (int i = nread; i < 512; i++)
				buffer[i] = '\0';
		}
		if (write(archive, buffer, 512) != 512)
			goto exit;
	}
	return 0;
exit:
	if (close(in) != 0) {
		printf("Close Erorr\n");
		error = -2;
	}
	return error;
}

struct header stat_to_header(const char *path,  const struct stat statFile) //26
{
	struct header header_files;
	memset(header_files.name, '\0', 100);
	memset(header_files.mode, '\0', 8);
	memset(header_files.uid, '\0', 8);
	memset(header_files.gid, '\0', 8);
	memset(header_files.size, '\0', 12);
	memset(header_files.mtime, '\0', 12);
	memset(header_files.checksum, '\0', 8);
	memset(header_files.linkflag, '\0', 1);
	memset(header_files.linkname, '\0', 100);
	if (strlen(path) > 99) { // 1 символ - '\0'
		printf("Не удалось(\n");
		//preparation for use link
	}
	strncpy(header_files.name, path, 100);
	ll_to_byte(statFile.st_mode, header_files.mode, 8);
	ll_to_byte(statFile.st_uid, header_files.uid, 8);
	ll_to_byte(statFile.st_gid, header_files.gid, 8);
	ll_to_byte(statFile.st_size, header_files.size, 12);
	ll_to_byte(statFile.st_mtimespec.tv_sec, header_files.mtime, 12);
	//Добавить контрольную сумму и можно будет записывать
	return header_files;
}

int write_header(int archive, const struct header file_header)
{
	if (write(archive, file_header.name, 100) != 100)
		return -1;
	if (write(archive, file_header.mode, 8) != 8)
		return -2;
	if (write(archive, file_header.uid, 8) != 8)
		return -3;
	if (write(archive, file_header.gid, 8) != 8)
		return -4;
	if (write(archive, file_header.size, 12) != 12)
		return -5;
	if (write(archive, file_header.mtime, 12) != 12)
		return -6;
	if (write(archive, file_header.checksum, 8) != 8)
		return -7;
	if (write(archive, file_header.linkflag, 1) != 1)
		return -8;
	if (write(archive, file_header.linkname, 100) != 100)
		return -9;
	if (putnull(archive, (512 - 257)) != (512 - 257))
		return -10;
	return 0;
}

int unarchive(const char *archive, const char *path) //36
{
	chdir(path);
	int in = open(archive, O_RDONLY);
	if (in == -1)
		goto exit_unarch;

	unsigned char buffer[512];
	int nnullblocks = 0;

	while (nnullblocks != 2) {
		// Check checksum

		size_t n_read_byte = read(in, buffer, 512);
		if (n_read_byte == -1ul)
			goto exit_unarch;
		if ( n_read_byte != 512)
			goto exit_unarch_demage;
		if (!isnull(buffer, 512)) {
			nnullblocks++;
			continue;
		}
		struct header file_header = byte_to_header((const char*) buffer);
		if (unarchive_file(file_header, in))
			return -1;
	}
	return 0;

exit_unarch_demage:
	close(in);
	printf("Архив поврежден.\n");
	return -1;
exit_unarch:
	perror(NULL);
	return -1;
}
int unarchive_file(const struct header file_header, int in) //29
{
	printf("OK\n");
	printf("Unarchive file: %s\n", file_header.name);
	int out = 0;
	if (S_ISDIR(byte_to_ll(file_header.mode, 8))) {
		if (create_ditr(file_header.name, file_header.mode) == -1)
			goto exit_funarch_any;
	} else {
		out = open(file_header.name, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
		if (out == -1)
			goto exit_funarch_any;
		if (fchmod(out, byte_to_ll(file_header.mode, 8)))
			goto exit_funarch_close;
		if (overwrite(in, out, 512, byte_to_ll(file_header.size, 12)) == -1)
			goto exit_funarch_close;
		close(out);
	}
	struct utimbuf time;
	time.modtime = byte_to_ll(file_header.mtime, 12);
	time.actime = byte_to_ll(file_header.mtime, 12);
	if (utime(file_header.name, &time))
		goto exit_funarch_any;
	return 0;
exit_funarch_close:
	close(out);
exit_funarch_any:
	perror(NULL);
	return -1;
}
int create_ditr (const char *name, const char mode[8])
{
	printf("Create dir: %s\n", name);
	if (mkdir(name, byte_to_ll(mode, 8)) != 0) {
		if (errno != 17) //Директория уже есть
			return -1;
	}
	return 0;
}
int overwrite (int in, int out, int nbuffer, unsigned long long size)
{
	char buffer[nbuffer];
	unsigned long long i = 0ll;
	for (i = 0; i < (size < (unsigned long long) nbuffer? 0 : size - (long long)nbuffer); i += nbuffer) {
		if (read(in, buffer, nbuffer) != nbuffer)
			return -1;
		if (write(out, buffer, nbuffer) != nbuffer)
			return -1;
	}
	if (read(in, buffer, nbuffer) != nbuffer)
		return -1;
	if (write(out, buffer, size % nbuffer) != size % nbuffer)
		return -1;
	return 0;
}
struct header byte_to_header(const char byte[512])
{
	struct header res_header;

	strncpy(res_header.name, byte, 100 - 1);
	res_header.name[99] = '\0';

	memcpy(res_header.mode, (byte + 100), 8);
	memcpy(res_header.uid, (byte + 108), 8);
	memcpy(res_header.gid, (byte + 116), 8);
	memcpy(res_header.size, (byte + 124), 12);

	memcpy(res_header.mtime, (byte + 136), 12);
	memcpy(res_header.checksum, (byte + 148), 8);
	memcpy(res_header.linkflag, (byte + 156), 1);
	memcpy(res_header.linkname, (byte + 157), 100);
	return res_header;
}
