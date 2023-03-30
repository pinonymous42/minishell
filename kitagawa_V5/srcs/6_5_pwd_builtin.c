/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_5_pwd_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 11:55:33 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/30 17:59:09 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	pwd_builtin(t_info *info)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
	{
		while (info->list != NULL)
		{
			if (ft_strcmp(info->list->key, "PWD") == 0)
			{
				write(1, info->list->value, ft_strlen(info->list->value));
				break ;
			}
			info->list = info->list->next;
		}
	}
}
