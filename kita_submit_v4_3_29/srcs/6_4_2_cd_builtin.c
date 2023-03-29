/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_4_2_cd_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:29:30 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/29 23:11:54 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void	norm_set_old_and_new_pwd(t_environ *tmp, \
	char *new_pwd_dup, t_environ *list)
{
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->key, "OLDPWD") == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(search_env("PWD", list));
			break ;
		}
		tmp = tmp->next;
	}
	tmp = list;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->key, "PWD") == 0)
		{
			free(tmp->value);
			tmp->value = new_pwd_dup;
			break ;
		}
		tmp = tmp->next;
	}
}

void	set_old_and_new_pwd(t_environ *list, char *new_pwd_dup)
{
	t_environ	*tmp;

	tmp = list;
	norm_set_old_and_new_pwd(tmp, new_pwd_dup, list);
}
