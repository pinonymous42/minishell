#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
  pid_t pid, result, pid_value;
  int status, code;
  
  // printf(">%d<\n", pid);
  pid = fork();

  printf("|process ID is %d|\n", pid);
  // fork失敗
  if(pid == -1){
    fprintf(stderr, "Error\n\n");
  }
  
// 子プロセスの処理
  if(pid == 0){
	// pid_value = getpid();
	// printf("process ID: %d\n", pid_value);
    while(1){
      printf("Child Process\n");
      sleep(1);
    }
  }
  else{
    sleep(3);
    
    kill(pid, SIGKILL);

    result = wait(&status);
    
    if(result < 0){
      fprintf(stderr, "Wait Error.\n\n");
      exit(-1);
    }

    // シグナルによる終了
    if (WIFSIGNALED(status)) {
      printf("シグナル番号：%dによる終了。\n",WTERMSIG(status));
    }
    
  }

  return 0;
}