# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

//環境変数がよくわからないな。。。
/*
Q：環境変数とはどういう時に使いますか？
A:
環境変数は、プログラム内で使用する情報を格納するために使われます。例えば、システムのホームディレクトリ、ログファイルのパス、一時ディレクトリなどがこれに該当します。
これらの変数は、システム全体で利用されるため、各プログラムが独自に定義する必要がなく、他のプログラムからも参照することができます。
また、複数のプログラム間で共通の環境変数を使用することで、設定情報を一元管理することができます。
これらの環境変数は、一般的にシステムレベルで定義され、プログラムによって参照されますが、
特定のプログラムに対して環境変数を明示的に設定することもできます（例えば、execve関数の引数として指定することができます）。
by Chat GPT。
*/

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