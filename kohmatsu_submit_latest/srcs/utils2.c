/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 00:03:28 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/29 14:11:23 by kohmatsu         ###   ########.fr       */
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
