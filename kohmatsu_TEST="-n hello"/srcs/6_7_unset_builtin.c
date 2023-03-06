/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_7_unset_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:28:52 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/05 18:34:34 by yokitaga         ###   ########.fr       */
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
		if (ft_strncmp(curr->key, key, ft_strlen(key)) == 0)
		{
			if (prev != NULL)
				prev->next = curr->next;
			else//prevがNULLの場合は、currが先頭の要素を指している
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

void unset_builtin(t_info *info, t_environ *list)
{
	int	i;

	i = 1;
	while (info->argv[i] != NULL)
	{
		unset_var(list, info->argv[i]);
		i++;
	}
}