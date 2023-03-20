/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_1_1_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 01:40:55 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/18 11:15:49 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	check_variable_character(char **var, char **p)
{
	*var = ft_calloc(sizeof(char), 1);
	while (is_variable_character(**p))
	{
		append_char(var, **p);
		*p += 1;
	}
}

void	append_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = 2;
	if (*s)
		size += ft_strlen(*s);
	new = x_str_malloc(size);
	if (*s)
		ft_strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}

int	is_variable_character(char c)
{
	return (ft_isalnum(c) || c == '_');
}

void	quote_removal(t_token *tok, t_environ *list)
{
	char	*new_word;
	char	*p;

	if (tok == NULL || (tok->kind != TOKEN_WORD
			&& tok->kind != TOKEN_METACHAR) || tok->word == NULL)
		return ;
	p = tok->word;
	new_word = NULL;
	while (*p)
	{
		if (*p == SINGLE_QUOTE && *(p + 1) != SINGLE_QUOTE)
			remove_single_quote(&p, &new_word);
		else if ((*p == DOUBLE_QUOTE && *(p + 1) == DOUBLE_QUOTE) \
			|| (*p == SINGLE_QUOTE && *(p + 1) == SINGLE_QUOTE))
			handle_empty_str(&p, &new_word);
		else if (*p == DOUBLE_QUOTE)
			remove_double_quote(&p, &new_word, list);
		else if (*p == '$')
			expand_variable(&p, &new_word, list);
		else
			just_append(tok, &p, &new_word);
	}
	free(tok->word);
	tok->word = new_word;
	quote_removal(tok->next, list);
}

char	**expand(t_token *tok, t_environ *list)
{
	char	**array;
	int		heredoc_count;
	int		i;
	int		heredoc_flag;

	i = 0;
	quote_removal(tok, list);
	array = token_list_to_array(tok);
	heredoc_count = count_heredoc(array);
	heredoc_flag = 0;
	while (array[i])
	{
		if (ft_strcmp(array[i], "<<") == 0 && heredoc_flag == 0)
		{
			if (heredoc_count == 1)
				write_to_heredoc_one(array, i, list);
			else
				write_to_heredoc_not_one(array, i, &heredoc_flag, list);
		}
		i++;
	}
	return (array);
}
