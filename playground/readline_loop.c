/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 19:28:13 by yokitaga          #+#    #+#             */
/*   Updated: 2023/01/27 22:02:30 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
    char *line;
    
    rl_outstream = stderr;
    while (1)
    {
        line = readline("minishell$");
        if (line == NULL)
            break;
        if (*line)
            add_history(line);
        printf("%s", line);
        free(line);
    }
    exit(0);
}

/*
実行結果①
minishell$aaa
minishell$aaa
minishell$aaa
minishell$aaaaaaaa
minishell$123445
minishell$123445
minishell$
minishell$
minishell$
minishell$^Daaaaaaaaaaaaaaaaa123445123445bash-3.2$ 
*/