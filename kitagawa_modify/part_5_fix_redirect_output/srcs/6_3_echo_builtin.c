/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_3_echo_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 13:07:12 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/09 13:09:03 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    echo_builtin(t_info *info)
{
    int option_flag;
    int i;

    // i = 0;
    // while (info->argv[i])
    // {
    //     printf(">%s<\n", info->argv[i]);
    //     i++;
    // }
    // exit(1);
    option_flag = 0;
    //printf("%s, %d\n", __FILE__, __LINE__);
    if (ft_strncmp(info->argv[1], "-n", 2) == 0)
        option_flag = 1;
    if (option_flag == 0)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        i = 1;
        while (info->argv[i])
        {
            if (ft_strncmp(info->argv[i], NO_SUCH_ENV, ft_strlen(NO_SUCH_ENV)) != 0)//環境変数がNO_SUCH_ENVでない場合は出力
                ft_putstr_fd(info->argv[i], STDOUT);
            else//環境変数がNO_SUCH_ENVの場合は何もせずに次の引数へ
            {
                i++;
                continue;
            } 
            if (info->argv[i + 1] != NULL)
                write(1, " ", 1);
            i++;
        }
        write(1, "\n", 1);
    }
    else
    {
        i = 2;
        //printf("%s, %d\n", __FILE__, __LINE__);
        while (info->argv[i])
        {
            //printf("%s\n", info->argv[i]);
            if (ft_strncmp(info->argv[i], NO_SUCH_ENV, ft_strlen(NO_SUCH_ENV)) != 0)
                ft_putstr_fd(info->argv[i], STDOUT);
            else//環境変数がない場合は何もせずに次の引数へ
            {
                i++;
                continue;
            } 
            if (info->argv[i + 1] != NULL)
                write(1, " ", 1);
            i++;
        }
    }
}