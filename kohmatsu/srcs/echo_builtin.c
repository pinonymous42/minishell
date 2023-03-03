/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 13:07:12 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/03 13:41:19 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    echo_builtin(t_info *info)
{
    int option_flag;
    int i;

    option_flag = 0;
    if (ft_strncmp(info->argv[1], "-n", 2) == 0)
        option_flag = 1;
    if (option_flag == 0)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        i = 1;
        while (info->argv[i])
        {
            ft_putstr_fd(info->argv[i], STDOUT);
            if (info->argv[i + 1] != NULL)
                write(1, " ", 1);
            i++;
        }
        write(1, "\n", 1);
    }
    else
    {
        i = 2;
        while (info->argv[i])
        {
            ft_putstr_fd(info->argv[i], STDOUT);
            if (info->argv[i + 1] != NULL)
                write(1, " ", 1);
            i++;
        }
    }
}