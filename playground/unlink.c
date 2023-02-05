#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
 
#define SHADOW_FILEPATH    "./tmp/my_shadow.txt"
 
int main(void)
{
  int fd;
 
  fd = open(SHADOW_FILEPATH, (O_RDWR | O_CREAT), 0664);
  if (fd < 0)
  {
	perror("./tmp/my_shadow.txt");
    return (1);
  }
  if (unlink(SHADOW_FILEPATH) < 0)
  {
    perror("unlink");
    close(fd);
    return -1;
  }
 
  /*
    処理を書く。
    自プロセスからはアクセス可能
    他プロセスからはファイルが見えない
  */
 
  close(fd); /* ここでファイルの実態が削除される */
  return 0;
}