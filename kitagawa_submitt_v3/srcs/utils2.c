/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 00:03:28 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/18 13:12:20 by kohmatsu         ###   ########.fr       */
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
	while (info->cmd[start][*i])
	{
		if (info->cmd[start][*i] == ' ')
		{
			*count += 1;
			while (info->cmd[start][*i] == ' ')
				*i += 1;
		}
		else
			*i += 1;
	}
}
