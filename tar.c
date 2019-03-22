
int archive(const char* archive_name, const char* path)
{
  // Открыть и создать файл архива
  int out = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
  getFiles(path, out, write_archive);
  if (close(out) != 0)
      return -1
  return 0
};
int get_files(const char* path, int archive_file, int (*func)(int, const char*))
{

};
int write_archive(int tarFile, const char* file_name)
{

};
int write_header(int tar_file, const char* file_name, struct stat statFile)
{

};

int unarchive(const char* path_archive, const char* path)
{

};
