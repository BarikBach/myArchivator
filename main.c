
#include <stdio.h>
#include <unistd.h>


#include <string.h>

int main(int argc, char const *argv[]) {
  /*
  * Структура вызова:
  * - Флаг: -a -u - разархивирование и архивирование соответсвтенно
  * - Путь или название архива
  * - Путь к каталогу для архивации
  * Итого три аргумента
  */

  if (agc < 3) {
    printf("Use ./lab -a <file> or ./lab -u <archive> <path> \n");
  };
  const char *path_archive = argv[2];
  const char *path_file = argv[3];
  if (!(strcmp("-a", argv[1])))
  {
    //Архивирование

  }
  if (!(strcmp("-u", argv[1])))
  {
    // разархивирование
  };

  return 0;
}
