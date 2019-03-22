#ifndef tar_h
#define tar_h


#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>


struct tar_header{
  char name[100];
};

int archive(const char* archive_name, const char* path);
int get_files(const char* path, int archive_file, int (*func)(int, const char*));
int write_tar(int tarFile, const char* file_name);
int write_header(int tar_file, const char* file_name, struct stat statFile);

int unarchive(const char* path_archive, const char* path);
#endif
