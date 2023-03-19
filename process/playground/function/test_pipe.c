#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main()
{
	int pipefd[2];
	if (pipe(pipefd) < 0) {
		perror("pipe");
		exit(-1);
	}

	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(-1);
	} else if (pid == 0) {
		// 子プロセス
		char *s = "send from child";
		write(pipefd[1], s, strlen(s));

//		sleep(1);

		char buf[128];
		read(pipefd[0], buf, sizeof buf);
		printf("child=[%s]\n", buf);

		close(pipefd[0]);
		close(pipefd[1]);
		return 0;
	} else {
		// 親プロセス
		sleep(1);
		char *s = "send from parent";
		write(pipefd[1], s, strlen(s));

		char buf[128];
		read(pipefd[0], buf, sizeof buf);
		printf("parent=[%s]\n", buf);

		close(pipefd[0]);
		close(pipefd[1]);
		return 0;
	}
}