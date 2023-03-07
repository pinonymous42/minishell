/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:53:49 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/03 16:25:28 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    exit_builtin(t_info *info)
{
    int status_code;
    int i;

    status_code = 0;
    i = 0;
    if (info->argv[1] != NULL)
    {
        while (info->argv[1][i])
        {
            if (ft_isdigit(info->argv[1][i]) == 0)
            {
                ft_putendl_fd("exit", STDERR_FILENO);
                ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
                ft_putstr_fd(info->argv[1], STDERR_FILENO);
                ft_putendl_fd(": numeric argument required", STDERR_FILENO);
                g_signal.status = 255;
                exit(255);
            }
            i++;
        }
        if (info->argv[2] != NULL)
        {
            ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
            g_signal.status = 1;
            g_signal.other_code = TRUE;
            return ;
        }
        status_code = ft_atoi(info->argv[1]);
    }
    g_signal.status = status_code;
    exit(status_code);
}