#include <stdio.h>
#include <signal.h> /* signal.hをインクルード */
#include <stdlib.h>
#include <string.h>

int count;

void signal_handler(int signum)
{
    /* シグナルハンドラ内で安全ではない関数を使っていない */
    count = 103;
}

int main(void){

    struct sigaction sa;
    /* シグナルマスクのクリア(エラーチェック付き) */
    if(-1 == sigemptyset(&sa.sa_mask)){
        exit(1);
    }
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;

    /* シグナルハンドラの登録(エラーチェック付き) */
    if(-1 == sigaction(SIGINT, &sa, NULL)){
        exit(1);
    }

    /* 変数countが50以下の間ループ */
    while(count < 50){
		// printf("%s, %d\n", __FILE__, __LINE__);
		printf("%d\n", count);
    }

    puts("Count Over.");
	printf("%d\n", count);

    return 0;
}