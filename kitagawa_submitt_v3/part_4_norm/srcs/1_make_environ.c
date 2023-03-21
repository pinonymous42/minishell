/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_make_environ.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 11:08:05 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/13 15:33:49 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_environ	*new_list(char *envp)
{
	t_environ	*new;
	char		**tmp;

	new = malloc(sizeof(t_environ));
	if (!new)
		function_error("malloc");
	tmp = ft_split(envp, '=');
	new->key = tmp[0];
	new->value = tmp[1];
	new->next = NULL;
	free(tmp);
	return (new);
}

void	list_add_back(t_environ **list, t_environ *new)
{
	t_environ	*head;

	head = *list;
	while (head->next != NULL)
		head = head->next;
	head->next = new;
}

t_environ	*make_environ(char **envp)
{
	t_environ	*list;

	if (*envp == NULL)
	{
		list = malloc(sizeof(t_environ));
		if (!list)
			function_error("malloc");
		list->key = NULL;
		list->value = NULL;
		list->next = NULL;
		return (list);
	}
	list = new_list(*envp++);
	while (*envp)
	{
		list_add_back(&list, new_list(*envp));
		envp++;
	}
	return (list);
}
