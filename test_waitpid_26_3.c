/*
waitpid()により子プロセスの状態を知る 
*/
# include <sys/wait.h>
# include <time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>

int main(int argc, char *argv[])
{
	int		status;
	pid_t	childpid;

	switch(fork()){
		case -1:
			printf("Error\n");
			exit(EXIT_FAILURE);
		case 0:
			printf("child");
		default:
			for (; ;){

			}
	}
}

