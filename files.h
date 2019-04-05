#ifndef FILES_H
#define FILES_H


char *get_new_path(const char *path, const char *name);
int get_files(const char *dir, const char *path, int archive_file,
				int (*func)(int, const char*,
				const char *, const struct stat));
int create_dir(const char *name, const char mode[8]);
int copy_data(int in, int out, int nbuffer, unsigned long long size);

#endif
