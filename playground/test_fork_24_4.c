/*
TLPI 24_4 fork後の競合状態
fork後にメッセージ表示を競合する親子プロセスのテストケース
fork後の実行は99.97%親プロセスが先。→重要
*/

# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

int main(int argc, char *argv[])
{
	int		numChild, j;
	pid_t	Childpid;

	numChild = atoi(argv[1]);

	setbuf(stdout, NULL);

	for (j = 0; j < numChild; j++)
	{
		switch (Childpid = fork()){
		case -1:
			printf("Error\n");
			exit(EXIT_FAILURE);
		case 0:
			printf("%d child\n", j);
			exit(EXIT_SUCCESS);
		default:
			printf("%d parent\n", j);
			wait(NULL);
			break;
		}
	}
	exit(EXIT_SUCCESS);
}
