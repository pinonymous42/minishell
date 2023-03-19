/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 00:04:56 by matsushimak       #+#    #+#             */
/*   Updated: 2023/02/19 20:54:46 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
#define PIPEX_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "../libft/libft.h"
#include "../libgnl/get_next_line.h"
#include <fcntl.h>

#define STDIN 0
#define STDOUT 1

typedef struct s_info{
    int     input_fd;
    int     output_fd;
    int     argc;//cmdの数
    int     pipe_count;//(pipeの個数)
    char    **cmd;//実行コマンド(全部)
    char    **argv;//実行コマンド(部分的)
    int     argv_count;//実行コマンドの数(部分的)
    char    **path;//search path
    char    **envp;//環境変数
    int     *pipe_place;//pipeの位置インデックス
    // char    *heredoc_str;
}t_info;

#endif
