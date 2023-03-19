#include <unistd.h>
 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
 
int main(void) {
  int fd;
  char *argv[3];
  extern char **environ;
 
  /* リダイレクト先のファイルを用意 */
  if ((fd = open("test.txt", (O_WRONLY | O_CREAT | O_TRUNC), 0666)) < 0) {
    perror("open");
    return -1;
  }
 
  /* 1=標準出力 に割り当てる */
  if (dup2(fd, 1) < 0) {
    perror("dup2");
    close(fd);
    return -1;
  }
//   close(fd);
 
  /* echo を実行する */
  argv[0] = "echo";
  argv[1] = "Hello World!";
  argv[2] = NULL;
  execve("/bin/echo", argv, environ);
 
  perror("execve");
// write(1, "abcdef", 6);
  close(fd);
  return (0);
}