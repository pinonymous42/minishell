/*
複数の子プロセスの作成と監視
*/

# include <sys/wait.h>
# include <time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>

/*----------------------------------------------------------------
実行結果
(base) KitagawaYoshifumi@kitagawamasaakinoMacBook-Air playground % ./a.out 7 1 2 5    
[1675329457] child 1 started with PID 84888, sleeping 7 seconds
[1675329457] child 2 started with PID 84889, sleeping 1 seconds
[1675329457] child 3 started with PID 84890, sleeping 2 seconds
[1675329457] child 4 started with PID 84891, sleeping 5 seconds
[1675329458] wait() returned child PID 84889 (numDead = 1 )
[1675329459] wait() returned child PID 84890 (numDead = 2 )
[1675329462] wait() returned child PID 84891 (numDead = 3 )
[1675329464] wait() returned child PID 84888 (numDead = 4 )
No more children --bye
*/
int main(int argc, char *argv[])
{
	int		numDead;	//現在までwait()した子プロセスの数
	pid_t	childpid;	//waitした子プロセスID
	int		j;
	time_t 	current_time;

	for (j = 1; j < argc; j++){
		switch(fork()){
			case -1:
				printf("fork error\n");
				exit(EXIT_FAILURE);
			case 0:
				printf("[%ld] child %d started with PID %ld, sleeping %s seconds\n", time(&current_time), j, (long)getpid(), argv[j]);
				sleep(atoi(argv[j]));
				exit(EXIT_SUCCESS);
			default:
				break;
		}
	}
	numDead = 0;
	for (;;){
		childpid = wait(NULL);
		if (childpid == -1){
			if (errno == ECHILD){
				printf("No more children --bye\n");
				exit(EXIT_SUCCESS);
			}
			else{
				printf("Error wait\n");
				exit(EXIT_FAILURE);
			}
		}
		numDead++;
		printf("[%ld] wait() returned child PID %ld (numDead = %d )\n", time(&current_time), (long)childpid, numDead);
	}
}