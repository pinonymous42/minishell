/*
execve()によるプログラム実行
*/

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

int main(int argc, char *argv[])
{
	char *argVec[10];
	char *envVec[] = {" GREET=salut", "BYE=adieu", NULL};

	if (argc != 2)
		exit(EXIT_FAILURE);
	argVec[0] = strrchr(argv[1], '/');
	if (argVec[0] != NULL)
		argVec[0]++;
	else
		argVec[0] = argv[1];
	argVec[1] = "hello world";
	argVec[2] = "goodbye";
	argVec[3] = NULL;

	execve(argv[1], argVec, envVec);
	exit(EXIT_SUCCESS);
}