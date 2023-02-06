#include <stdio.h>
int main(int argc, char**argv) {
  FILE *file;
  file = fopen("hoge.txt", "r");
  if (file == NULL) {
    perror("a");
    return 1;
  }
  fclose(file);
  return 0;
}