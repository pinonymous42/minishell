/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_4_2_cd_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:29:30 by yokitaga          #+#    #+#             */
/*   Updated: 2023/04/01 15:20:27 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	norm_set_old_and_new_pwd(bool flag, char *new_pwd_dup, t_environ *list)
{
	t_environ	*tmp;

	tmp = list;
	if (flag == FALSE)
	{
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
		tmp = list;
		while (tmp != NULL)
		{
			if (ft_strcmp(tmp->key, "COPYPWD") == 0)
			{
				free(tmp->value);
				tmp->value = ft_strdup(search_env("PWD", list));
				break ;
			}
			tmp = tmp->next;
		}
	}
	else if (flag == TRUE)
	{
		tmp = list;
		while (tmp != NULL)
		{
			if (ft_strcmp(tmp->key, "COPYPWD") == 0)
			{
				free(tmp->value);
				tmp->value = new_pwd_dup;
				break ;
			}
			tmp = tmp->next;
		}
	}
}

void	set_old_and_new_pwd(t_environ *list, char *new_pwd_dup)
{
	t_environ	*tmp;
	static	bool	flag = FALSE;

	tmp = list;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->key, "OLDPWD") == 0)
		{
			if (search_env("PWD", list) != NULL)
			{
				free(tmp->value);
				tmp->value = ft_strdup(search_env("PWD", list));
			}
			else//PWDからコピーできなかった場合
			{
				if (flag == FALSE)
				{
					flag = TRUE;
					free(tmp->value);
					tmp->value = ft_strdup("");
				}
				else
				{
					free(tmp->value);
					tmp->value = ft_strdup(search_env("COPYPWD", list));
				}
			}
			break ;
		}
		tmp = tmp->next;
	}
	norm_set_old_and_new_pwd(flag, new_pwd_dup, list);
}
