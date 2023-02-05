#include <stdio.h>
#include <string.h>
#include <errno.h>
int main(int argc, char**argv) {
  FILE *file;
  file = fopen("hoge.txt", "r");
  if (file == NULL) {
    fprintf(stderr, "%d: %s\n", errno, strerror(errno));
    return 1;
  }
  fclose(file);
  return 0;
}