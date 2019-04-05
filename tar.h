#ifndef TAR_H
#define TAR_H
#include <stdbool.h>
struct header {
	char	name[100];
	char	mode[8];
	char	uid[8];
	char	gid[8];
	char	size[12];
	char	mtime[12];
	char	checksum[8];
	char	linkflag[1];
	char	linkname[100];
	char	pad[255];
};
bool visible_progress;

int archive(const char *archive_name, const char *path);




int write_archive(int archive, const char *file_name, const char *path,
				   struct stat file_stat);
struct header stat_to_header(const char *path,  struct stat statFile);
int write_header(int archive, const struct header file_header);
struct header byte_to_header(const char byte[512]);
int unarchive(const char *archive, const char *path);
int unarchive_file(const struct header file_header, int in);






#endif
