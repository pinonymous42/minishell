/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_3_echo_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 13:07:12 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/14 22:47:26 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	no_option(t_info *info, int j)
{
	int	i;
	int k;

	i = j + 1;
	while (info->argv[i])
	{
		if (ft_strchr(info->argv[i], '$') == NULL)
		{
			if (info->argv[i] != NULL)
				ft_putstr_fd(info->argv[i], STDOUT);
		}
		else if (g_signal.not_expand_flag == 1)
			ft_putstr_fd(info->argv[i], STDOUT);
		else
		{
			k = 0;
			while (info->argv[i][k] != '$')
			{
				write(1, &info->argv[i][k], 1);
				k++;
			}
		}
		if (info->argv[i + 1] != NULL)
			write(1, " ", 1);
		i++;
	}
	write(1, "\n", 1);
}

void	option(t_info *info, int j)
{
	int	i;

	i = j + 2;
	while (info->argv[i])
	{
		if (ft_strchr(info->argv[i], '$') == NULL)
		{
			if (info->argv[i] != NULL)
				ft_putstr_fd(info->argv[i], STDOUT);
		}
		else
		{
			i++;
			continue ;
		}
		if (info->argv[i + 1] != NULL)
			write(1, " ", 1);
		i++;
	}
}

void	echo_builtin(t_info *info, int j)
{
	int	option_flag;

	option_flag = 0;
	if (ft_strncmp(info->argv[1], "-n", 2) == 0)
		option_flag = 1;
	if (option_flag == 0)
		no_option(info, j);
	else
		option(info, j);
}
