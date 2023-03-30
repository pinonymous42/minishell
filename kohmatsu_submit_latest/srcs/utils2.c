/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 00:03:28 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/31 00:10:43 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_space(char *str)
{
	while (*str)
	{
		if (*str != ' ')
			return (0);
		str++;
	}
	return (1);
}

void	move_space(t_info *info, int start, int *i, int *count)
{
	int	j;

	while (info->cmd[start][*i])
	{
		if (info->cmd[start][*i] == ' ')
		{
			j = 0;
			while (info->cmd[start][*i + j] == ' ')
				j++;
			if (info->cmd[start][*i + j])
				*count += 1;
			while (info->cmd[start][*i] == ' ')
				*i += 1;
		}
		else
			*i += 1;
	}
}

void	cut_wstatus(int wstatus)
{
	if (wstatus == SIGINT)
		write(1, "\n", 1);
	else if (wstatus == SIGQUIT)
		write(1, "QUIT: 3\n", 8);
	if (wstatus == SIGINT || wstatus == SIGQUIT)
	{
		g_signal.status = 128 + wstatus;
		g_signal.other_code = TRUE;
	}
	else
	{
		g_signal.status = WEXITSTATUS(wstatus);
		g_signal.other_code = TRUE;
	}
}

char	*x_strndup(char *str, int i)
{
	char	*tmp;

	tmp = ft_strndup(str, i);
	if (tmp == NULL)
		function_error("ft_strndup");
	return (tmp);
}
