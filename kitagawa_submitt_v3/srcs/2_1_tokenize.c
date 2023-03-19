/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_1_tokenize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 15:03:28 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/19 20:34:40 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	token_handle(t_token **head, char **rest, char *line)
{
	t_token	*token;

	if (is_metacharacter(*line) == true)
	{
		token = handle_metacharacter(rest, line);
		append_token(head, token);
	}
	else if (is_control_operator(line) == true)
	{
		token = handle_contorol_op(rest, line);
		append_token(head, token);
	}
	else
	{
		token = handle_word(rest, line);
		append_token(head, token);
	}
}

t_token	*tokenize(char *line)
{
	t_token	*head;
	t_token	*tmp;

	head = NULL;
	while (*line != '\0')
	{
		if (is_blank(*line) == true)
		{
			while (is_blank(*line) == true)
				line++;
		}
		token_handle(&head, &line, line);
	}
	check_and_sort_tokens(&head);
	check_pipe_place(&head);
	append_token(&head, new_token(TOKEN_EOF));
	tmp = head;
	while (tmp->kind != TOKEN_EOF)
	{
		if (!ft_strcmp(tmp->word, "|") && !ft_strcmp(tmp->next->word, "|"))
			tokenize_error_2("\0", "|");
		tmp = tmp->next;
	}
	return (head);
}
