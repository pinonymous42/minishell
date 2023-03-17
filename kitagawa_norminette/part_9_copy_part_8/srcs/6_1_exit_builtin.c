/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_1_exit_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:53:49 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/17 13:52:39 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	error_exit(char *str)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	g_signal.status = 255;
	exit(255);
}

void	exit_builtin(t_info *info)
{
	int	status_code;
	int	i;

	status_code = 0;
	i = 0;
	if (info->argv[1] != NULL)
	{
		while (info->argv[1][i])
		{
			if (ft_isdigit(info->argv[1][i]) == 0 && info->argv[1][i] != '-')
				error_exit(info->argv[1]);
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
		g_signal.status = status_code;
	}
	else
		status_code = g_signal.status;
	exit(status_code);
}
