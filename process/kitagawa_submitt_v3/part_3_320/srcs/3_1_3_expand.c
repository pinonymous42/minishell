/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_1_3_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 19:16:13 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/21 12:21:39 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	remove_double_quote(char **p, char **new_word, t_environ *list)
{
	*p += 1;
	while (**p != DOUBLE_QUOTE)
	{
		if (**p == '\0')
			assert_error("Unclosed single quote");
		append_char(new_word, **p);
		*p += 1;
	}
	*p += 1;
	while (ft_strchr(*new_word, '$'))
		*new_word = double_variable_expand(*new_word, list);
}

void	expand_status(char **p, char **new_word)
{
	*p += 1;
	*new_word = ft_itoa(g_signal.status);
}

void	expansion(char **p, char **new_word, t_environ *list)
{
	char	*var;

	var = NULL;
	if (ft_strchr(*p, '$'))
		var = ft_strndup(*p, ft_strchr(*p, '$') - *p);
	else
	{
		if (**p == ' ' || **p == '\0')
		{
			var = x_strdup("\0");
			g_signal.not_expand_flag = 1;
		}
		else if (ft_isdigit(**p))
		{
			while (**p != ' ' && **p != '\0')
			{
				append_char(&var, **p);
				g_signal.not_expand_flag = 1;
				*p += 1;
			}
		}
		else if (not_allowed_variant_character(*p))
			check_variable_character(&var, p);
		else
			var = ft_strndup(*p, ft_strchr(*p, '\0') - *p);
	}
	if (*new_word == NULL)
		*new_word = create_new_word_if_null(var, list);
	else
		create_new_word_if_not_null(new_word, var, list);
	free(var);
}

void	expand_variable(char **p, char **new_word, t_environ *list)
{
	g_signal.do_split = 1;
	while (ft_strchr(*p, '$'))
	{
		*p += 1;
		if (**p == '?')
			expand_status(p, new_word);
		else if (**p == DOUBLE_QUOTE && *(*p + 1) == DOUBLE_QUOTE)
			handle_empty_str(p, new_word);
		else if (**p == SINGLE_QUOTE && *(*p + 1) == SINGLE_QUOTE)
			handle_empty_str(p, new_word);
		else if (**p == DOUBLE_QUOTE)
			remove_double_quote(p, new_word, list);
		else if (**p == SINGLE_QUOTE)
			remove_single_quote(p, new_word);
		else
		{
			expansion(p, new_word, list);
			if (ft_strchr(*p, '$'))
				*p = ft_strchr(*p, '$');
			else
			{
				if (not_allowed_variant_character(*p))
				{
					while (is_variable_character(**p))
						*p += 1;
				}
				else
					*p = ft_strchr(*p, '\0');
			}
		}
	}
}

void	just_append(t_token *tok, char **p, char **new_word)
{
	if (ft_strcmp(tok->word, "<<") == 0 && ft_strchr(tok->next->word, '\"')
		&& ft_strcmp(tok->next->next->word, "<<") == 1)
		g_signal.not_expand_flag = 1;
	append_char(new_word, **p);
	*p += 1;
}
