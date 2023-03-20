/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_6_1_export_builtin.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 20:28:47 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/19 23:50:08 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	put_env(t_environ *list)
{
	t_environ	*tmp;

	tmp = list;
	while (tmp != NULL)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(tmp->key, 1);
		if (tmp->value != NULL)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(tmp->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
	}
}

char	*make_key(char *argv)
{
	char	*key;

	if (ft_strchr_index(argv, '=') != -1)
		key = ft_strndup(argv, ft_strchr_index(argv, '='));
	else
		key = ft_strdup(argv);
	return (key);
}

bool	judge_key(char *key, char *argv, t_environ *list)
{
	if (not_allowed_variant_character(key) || ft_isdigit(key[0]) == 1)
		export_not_valid_identifier(argv);
	if (ft_strchr(key, '$') != NULL)
		return (false);
	else if (search_env(key, list) != NULL)
		update_env(argv, list);
	else if (search_env(key, list) == NULL
		&& !not_allowed_variant_character(key))
		add_new_env(argv, list);
	return (true);
}

void	handle_argv(t_info *info, t_environ *list)
{
	int		i;
	char	*key;

	i = 1;
	while (info->argv[i] != NULL)
	{
		if (check_add_or_not(info->argv[i], list) == true)
			add_env_value(info->argv[i], list);
		else
		{
			key = make_key(info->argv[i]);
			if (judge_key(key, info->argv[i], list) == false)
			{
				if (yatto_owari(&key, &i))
					continue ;
				break ;
			}
			free(key);
		}
		i++;
	}
}

void	export_builtin(t_info *info, t_environ *list)
{
	if ((info->argv[1] == NULL && info->argv_count == 2)
		|| check_argv_no_such_env(info) == true)
		put_env(list);
	else
		handle_argv(info, list);
	return ;
}
