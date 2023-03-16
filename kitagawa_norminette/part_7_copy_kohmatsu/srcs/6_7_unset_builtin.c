/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_7_unset_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:28:52 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/16 11:32:42 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	unset_var(t_environ *list, char *key)
{
	t_environ	*prev;
	t_environ	*curr;

	prev = NULL;
	curr = list;
	while (curr != NULL)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev != NULL)
				prev->next = curr->next;
			else
				list = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	unset_builtin(t_info *info, t_environ *list)
{
	int	i;

	i = 1;
	while (info->argv[i] != NULL)
	{
		if (not_allowed_variant_character(info->argv[i]))
			export_not_valid_identifier(info->argv[i], "unset");
		else
			unset_var(list, info->argv[i]);
		i++;
	}
}
