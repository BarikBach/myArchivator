
#include "bytes.h"
#include <stdlib.h>

int isnull(const unsigned char *arr, int nbyte)
{
	int nnonull = 0;

	for (int i = 0; i < nbyte; i++) {
		if (arr[i] != '\0')
			nnonull++;
	}
	return nnonull;
}

void ll_to_byte(long long val, char *arr, int nbyte)
{
	for (int i = 0; i < nbyte; i++)
		arr[nbyte - i - 1] = (val & (0xffLL<<((i) * 8)))>>((i) * 8);
}

long long byte_to_ll(const char *arr, int nbyte)
{
	long long res = 0;

	for (int i = nbyte - 1 ; i >= 0; i--)
		res = res | ((unsigned long long)((unsigned char) arr[i])<<
							(8*(nbyte - i - 1)));
	return res;
}

int putnull(int file, int n_null)
{
	char *nullbyte = (char *) calloc(n_null, sizeof(char));

	if (nullbyte == NULL) {
		perror(NULL);
		return -1;
	}
	int res = write(file, nullbyte, n_null);
	if (res == -1)
		perror(NULL);
	free(nullbyte);
	return res;
}
