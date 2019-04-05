#ifndef BYTES_H
#define BYTES_H

void ll_to_byte(long long val, char *arr, int nbyte);
long long byte_to_ll(const char *arr, int nbyte);

int isnull(const unsigned char *arr, int nbyte);
int putnull(int file, int n_null);

#endif
