# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

//環境変数がよくわからないな。。。
extern char **environ;

int main(int argc, char *argv[])
{
	int 	j;
	char 	**ep;

	for (j = 0; j < argc; j++)
		printf("argv[%d] = %s\n", j, argv[j]);
	
	for (ep = environ; *ep != NULL; ep++)
		printf("environ:%s\n", *ep);
	exit(EXIT_SUCCESS);
}

/*
test_execve_27_1.c = test_execve
test_execve_27_1.c = env
として実行ファイルを作成
./test_execve env
を実行すると

argv[0] = env
argv[1] = hello world
argv[2] = goodbye
environ: GREET=salut
environ:BYE=adieu
が出力された
*/