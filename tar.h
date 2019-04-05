#ifndef tar_h
#define tar_h


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


int archive(const char *archive_name, const char *path);
int get_files(const char *dir, char *path, int archive_file,
				int (*func)(int, const char*, const char *, const struct stat));
int write_archive(int archive, const char *file_name, const char *path,
				  const struct stat file_stat);

void ll_to_byte(long long val, unsigned char *arr, int nbyte);
int putnull(int file, int n_null);
long long byte_to_ll(unsigned char *arr, int nbyte);

struct header stat_to_header(const char *path, char *file_name, const struct stat statFile);
int write_header(int archive, struct header file_header);


struct header byte_to_header(const char byte[512]);
int unarchive(const char *archive, const char *path);


#endif
