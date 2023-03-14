/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_6_2_export_builtin.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:34:51 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/14 19:53:53 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strchr_index(const char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

t_environ	*new_node(char *key, char *value)
{
	t_environ	*new;

	new = malloc(sizeof(t_environ));
	if (!new)
		function_error("malloc");
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;
	return (new);
}

void	list_add_back_export(t_environ **list, char *key, char *value)
{
	t_environ	*head;

	head = *list;
	while ((*list)->next != NULL)
		*list = (*list)->next;
	(*list)->next = new_node(key, value);
	*list = head;
}

void	update_env(char *arg, t_environ *list)
{
	char	*key;
	char	*value;
	int		i;

	i = ft_strchr_index(arg, '=');
	if (i == -1)
		return ;
	else
	{
		key = ft_strndup(arg, i);
		if (key == NULL)
			function_error("strndup");
		value = ft_substr(arg, i + 1, ft_strlen(arg) - i - 1);
		while (list != NULL)
		{
			if (ft_strncmp(list->key, key, ft_strlen(key)) == 0)
			{
				free(list->value);
				list->value = value;
				break ;
			}
			list = list->next;
		}
	}
	free(key);
}

void	add_new_env(char *arg, t_environ *list)
{
	char	*key;
	char	*value;

	if (ft_strchr_index(arg, '=') == -1)
	{
		key = ft_strdup(arg);
		if (key == NULL)
			function_error("strdup");
		value = NULL;
		list_add_back_export(&list, key, value);
		free(key);
	}
	else
	{
		key = ft_strndup(arg, ft_strchr_index(arg, '='));
		if (key == NULL)
			function_error("strndup");
		value = ft_substr(arg, ft_strchr_index(arg, '=') + 1,
				ft_strlen(arg) - ft_strchr_index(arg, '=') - 1);
		if (value == NULL)
			function_error("substr");
		list_add_back_export(&list, key, value);
		free(key);
		free(value);
	}
}
