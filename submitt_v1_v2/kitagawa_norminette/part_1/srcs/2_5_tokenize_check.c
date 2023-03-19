/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_4_tokenize_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:54:55 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/13 18:58:28 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	check_redirect_token(t_token *token)
{
	if (token->kind == TOKEN_METACHAR)
	{
		if (ft_strcmp(token->word, ">") == 0 || \
			ft_strcmp(token->word, ">>") == 0 \
			|| ft_strcmp(token->word, "<") == 0 || \
				ft_strcmp(token->word, "<<") == 0)
			return (true);
	}
	return (false);
}

void	check_and_sort_tokens(t_token **head)
{
	t_token	*current;
	t_token	*prev;
	t_token	*tmp;

	current = *head;
	prev = NULL;
	while (current != NULL)
	{
		if (check_redirect_token(current) == true)
		{
			if (current->next == NULL || current->next->kind != TOKEN_WORD)
			{
				tokenize_error_2("unexpected token", current->word);
				break ;
			}
			else
			{
				tmp = current->next->next;
				if (tmp == NULL || tmp->kind != TOKEN_WORD)
					;
				else
				{
					while (tmp->kind == TOKEN_WORD)
					{
						if (tmp->next == NULL)
							break ;
						else if (tmp->next->kind != TOKEN_WORD)
							break ;
						tmp = tmp->next;
					}
					if (prev == NULL)
					{
						*head = current->next->next;
						if (tmp->next == NULL)
							current->next->next = NULL;
						else
							current->next->next = tmp->next;
						tmp->next = current;
					}
					else
					{
						prev->next = current->next->next;
						if (tmp->next == NULL)
							current->next->next = NULL;
						else
							current->next->next = tmp->next;
						tmp->next = current;
					}
				}
			}
		}
		prev = current;
		current = current->next;
	}
}
