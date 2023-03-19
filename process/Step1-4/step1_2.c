/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   step1_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 01:39:22 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/05 23:05:32 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "step1_4.h"

int main(void)
{
    char *line;
    
    /*
    rl_outstream を使ってreadlineの出力先をstderrに変更しました。
    macの標準で入っているreadlineはデフォルトで出力先がstderrなのでこれは必要ないのですが、
    GNUのreadlineは初期設定がstdoutになっているためです。
    これを標準出力のままにしておくと、テストで標準出力のdiffを比較する際にreadlineのpromptが出力されてしまって困るので、
    このように標準エラー出力に吐き出すように設定しています。
    */
    rl_outstream = stderr;
    while (1)
    {
        line = readline("minishell$");
        if (line == NULL)
            break;
        if (*line)
            add_history(line);
        free(line);
    }
    exit(0);
}