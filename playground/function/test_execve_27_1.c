/*
execve()によるプログラム実行
*/

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

/*----------------------------------------------------------------
execveはC言語のライブラリ関数で、新しいプログラムを実行するために使用されます。この関数の引数は次のとおりです。

filename: 新しいプログラムのパス名。
argv: 新しいプログラムへ渡す引数の文字列の配列。最初の要素は新しいプログラム名であり、残りの要素は引数となります。
envp: 新しいプログラムに与えられる環境変数の文字列の配列。各要素は「変数名=値」の形式です。
このように、execve関数は新しいプログラムに対する実行環境を指定することができます。
新しいプログラムは既存のプロセスの上に実行されますが、既存のプログラムのメモリ空間やデータ構造は失われます。→ほーん？？よくわからん。
*/
int main(int argc, char *argv[])
{
	char *argVec[10];
	char *envVec[] = {" GREET=salut", "BYE=adieu", NULL};

	if (argc != 2)
		exit(EXIT_FAILURE);
	argVec[0] = strrchr(argv[1], '/'); //./env→env。argVec[0] = env
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