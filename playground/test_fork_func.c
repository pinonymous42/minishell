# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

/*
pid_t	fork(void);
親プロセス：成功時は子プロセスのIDを、エラー時は-1を返す。
成功した場合の子プロセス：常に0を返す。
*/

/*TLPI P.545のテストケース*/

static int idata = 111;

int main (int argc, char *argv[])
{
	int istack = 222;
	pid_t	childpid;

	switch (childpid = fork())
	{
	case -1:
		printf("Error\n");
		exit(EXIT_FAILURE);
	case 0:
		idata *= 3;
		istack *= 3;
		break;
	default:
		sleep(3);
		break;
	}
	printf("PID=%ld %s idata=%d istack=%d\n", (long)getpid(), (childpid == 0) ? "(child)" : "(parent)", idata, istack);
	exit(EXIT_SUCCESS);
}