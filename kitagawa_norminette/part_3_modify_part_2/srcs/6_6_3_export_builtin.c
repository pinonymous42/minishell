/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_6_3_export_builtin.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 20:10:53 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/14 14:06:46 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	check_argv_no_such_env(t_info *info)
{
	int	i;

	i = 1;
	while (info->argv[i] != NULL)
	{
		if (ft_strchr(info->argv[i], '$') == NULL)
			return (false);
		i++;
	}
	return (true);
}

bool	check_add_or_not(char *arg, t_environ *list)
{
	int		i;
	char	*key;

	i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] == '+' && arg[i + 1] == '=')
		{
			key = ft_strndup(arg, i);
			if (search_env(key, list) != NULL)
			{
				free(key);
				return (true);
			}
			else
			{
				free(key);
				return (false);
			}
		}
		i++;
	}
	return (false);
}

void	add_env_value(char *arg, t_environ *list)
{
	int		i;
	char	*key;

	i = ft_strchr_index(arg, '+');
	key = ft_strndup(arg, i);
	while (list != NULL)
	{
		if (ft_strncmp(list->key, key, ft_strlen(key)) == 0)
		{
			list->value = ft_strjoin_with_free(list->value, arg + i + 2, 1);
			break ;
		}
		list = list->next;
	}
	free(key);
}

int	not_allowed_variant_character(char *key)
{
	while (*key)
	{
		if (!(ft_isalnum(*key) || *key == '_'))
			return (1);
		key++;
	}
	return (0);
}
