#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <utime.h>
#include <stdbool.h>

#include "bytes.h"
#include "tar.h"
#include "files.h"

bool visible_progress = false;

int archive(const char *archive_name, const char *path)
{
	int out = open(archive_name, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	if (out == -1) {
		perror("Ошибка файла архива");
		return -1;
	}
	const char *path1 = (char *) calloc(1, sizeof(char));
	if (path1 == NULL) {
		perror(NULL);
		return -1;
	}
	get_files(path, path1, out, write_archive);
	if (putnull(out, 1024) != 1024)
		return -1;
	if (close(out) != 0) {
		perror(NULL);
		return -1;
	}
	return 0;
};

int write_archive(int archive, const char *file_name, const char *path,
				  struct stat file_stat)
{
	if (write_header(archive, stat_to_header(path, file_stat)))
		return -1;
	if (S_ISDIR(file_stat.st_mode))
		return 0;
	int in = open(file_name, O_RDONLY);

	if (in == -1) {
		perror(NULL);
		return -1;
	}
	int nread;
	nread = copy_data(in, archive, 512, file_stat.st_size);
	if (nread == -1)
		goto exit;
	if (putnull(archive, 512 - nread) != (512 - nread))
		goto exit;

	if (visible_progress)
		printf("Archive files: %s\n", path);
	if (close(in) == -1) {
		perror(NULL);
		return -1;
	}
	return 0;
exit:
<<<<<<< HEAD
	if (close(in) != 0)
=======
	if (close(in) != 0) 
>>>>>>> f99c495fe81b198184fee63d28784bedfe4172ab
		perror(NULL);
	return -1;
}

struct header stat_to_header(const char *path,  struct stat statFile)
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
	// preparation for use checksum
	return header_files;
}

int write_header(int archive, const struct header file_header)
{
	if (write(archive, file_header.name, 100) != 100)
		goto exit_write_header;
	if (write(archive, file_header.mode, 8) != 8)
		goto exit_write_header;
	if (write(archive, file_header.uid, 8) != 8)
		goto exit_write_header;
	if (write(archive, file_header.gid, 8) != 8)
		goto exit_write_header;
	if (write(archive, file_header.size, 12) != 12)
		goto exit_write_header;
	if (write(archive, file_header.mtime, 12) != 12)
		goto exit_write_header;
	if (write(archive, file_header.checksum, 8) != 8)
		goto exit_write_header;
	if (write(archive, file_header.linkflag, 1) != 1)
		goto exit_write_header;
	if (write(archive, file_header.linkname, 100) != 100)
		goto exit_write_header;
	if (putnull(archive, (512 - 257)) != (512 - 257))
		goto exit_write_header;
	return 0;
exit_write_header:
	perror("Ошибка во время записи header");
	return -1;

}

int unarchive(const char *archive, const char *path)
{
	if (chdir(path))
		goto exit_unarch;
	int in = open(archive, O_RDONLY);
	if (in == -1)
		goto exit_unarch;

	unsigned char buffer[512];
	int nnullblocks = 0;

	while (nnullblocks != 2) { // read header
		// Check checksum
		size_t n_read_byte = read(in, buffer, 512);

		if (n_read_byte == -1ul)
			goto exit_unarch;
		if (n_read_byte != 512)
			goto exit_unarch_demage;
		if (!isnull(buffer, 512)) {
			nnullblocks++;
			continue;
		}

		struct header fheader = byte_to_header((const char *) buffer);

		if (unarchive_file(fheader, in))
			return -1;
	}
	return 0;

exit_unarch_demage:
	close(in);
	 write(stderr, "Архив поврежден.\n");
	return -1;
exit_unarch:
	perror(NULL);
	return -1;
}
int unarchive_file(const struct header file_header, int in) //29
{
	if (visible_progress)
		printf("Unarchive file: %s\n", file_header.name);
	int out = 0;

	if (S_ISDIR(byte_to_ll(file_header.mode, 8))) {

		if (create_dir(file_header.name, file_header.mode) == -1)
			goto exit_funarch_any;
	} else {

		out = open(file_header.name, O_WRONLY|O_CREAT|O_TRUNC,
					     S_IRUSR|S_IWUSR);
		if (out == -1)
			goto exit_funarch_any;
		if (copy_data(in, out, 512,
				byte_to_ll(file_header.size, 12)) == -1)
			goto exit_funarch_close;
		if (fchmod(out, byte_to_ll(file_header.mode, 8)))
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
