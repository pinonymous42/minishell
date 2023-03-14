/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_2_env_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 14:26:14 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/14 11:35:34 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	env_builtin(t_info *info)
{
	t_environ	*tmp;

	tmp = info->list;
	while (tmp)
	{
		if (ft_strchr_index(tmp->key, '=') != -1)
		{
			ft_putstr_fd(tmp->key, STDOUT);
			write(1, "=", 1);
			ft_putstr_fd(tmp->value, STDOUT);
			write(1, "\n", 1);
		}
		tmp = tmp->next;
	}
}
