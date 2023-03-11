/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_3_echo_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 13:07:12 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/11 15:07:44 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    echo_builtin(t_info *info, int j)
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
        i = j+1;
        while (info->argv[i])
        {
            if (ft_strchr(info->argv[i], '$') == NULL)//info->argv[i]に$が含まれていない場合はそのまま出力
            {
                // printf("%s, %d\n", __FILE__, __LINE__);
                if (ft_strncmp(info->argv[i], NO_SUCH_ENV, ft_strlen(NO_SUCH_ENV)))
                    ft_putstr_fd(info->argv[i], STDOUT);
            }
            else//info->argv[i]に$が含まれている場合は飛ばす
            {
                // printf("%s, %d\n", __FILE__, __LINE__);
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
        i = j+2;
        //printf("%s, %d\n", __FILE__, __LINE__);
        while (info->argv[i])
        {
            //printf("%s\n", info->argv[i]);
            if (ft_strchr(info->argv[i], '$') == NULL)
            {//argv[i]に$が含まれていない場合はそのまま出力
                if (ft_strncmp(info->argv[i], NO_SUCH_ENV, ft_strlen(NO_SUCH_ENV)))
                    ft_putstr_fd(info->argv[i], STDOUT);
            }
            else//argv[i]に$が含まれている場合は$を含む文字列を出力
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