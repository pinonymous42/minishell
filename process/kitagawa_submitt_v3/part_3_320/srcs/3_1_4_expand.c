/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_1_4_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 19:18:27 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/20 15:23:40 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	double_expansion(char **new_word, char **ret, t_environ *list)
{
	char	*var;

	var = ft_calloc(sizeof(char), 1);
	if (ft_strchr(*new_word, '$') || ft_strchr(*new_word, ' ')
		|| not_allowed_variant_character(*new_word))
	{
		while (**new_word != '$' && **new_word != ' '
			&& is_variable_character(**new_word))
		{
			append_char(&var, **new_word);
			*new_word += 1;
		}
	}
	else
	{
		free(var);
		var = ft_strndup(*new_word, ft_strchr(*new_word, '\0') - *new_word);
	}
	if (search_env(var, list))
		*ret = ft_strjoin_with_free(*ret, search_env(var, list), FIRST_PARAM);
	else if (ft_isdigit(*var))
	{
		*ret = ft_strjoin_with_free(*ret, (var + 1), FIRST_PARAM);
		g_signal.not_expand_flag = 1;
	}
	free(var);
}

void	double_moving(char **new_word)
{
	if (ft_strchr(*new_word, '$') || ft_strchr(*new_word, ' '))
	{
		if (ft_strchr(*new_word, ' ') == 0 || (ft_strchr(*new_word, '$')
				&& (ft_strchr(*new_word, '$') < ft_strchr(*new_word, ' '))))
			*new_word = ft_strchr(*new_word, '$');
		else if (ft_strchr(*new_word, '$') == 0 || (ft_strchr(*new_word, ' ')
				&& (ft_strchr(*new_word, '$') > ft_strchr(*new_word, ' '))))
			*new_word = ft_strchr(*new_word, ' ');
	}
	else
	{
		if (not_allowed_variant_character(*new_word))
		{
			while (is_variable_character(**new_word))
				*new_word += 1;
		}
		else
			*new_word = ft_strchr(*new_word, '\0');
	}
}

void	double_expand_status(char **ret, char **new_word)
{
	*ret = ft_strjoin_with_free(*ret, ft_itoa(g_signal.status), FIRST_PARAM);
	*new_word += 1;
}

char	*double_variable_expand(char *new_word, t_environ *list)
{
	char	*tmp;
	char	*ret;

	g_signal.do_split = 1;
	tmp = new_word;
	while (*new_word != '$')
		new_word++;
	if (new_word == tmp)
		ret = ft_calloc(sizeof(char), 1);
	else
		ret = ft_strndup(tmp, new_word - tmp);
	new_word++;
	if (*new_word == '?')
		double_expand_status(&ret, &new_word);
	else
	{
		double_expansion(&new_word, &ret, list);
		double_moving(&new_word);
	}
	ret = ft_strjoin_with_free(ret, new_word, FIRST_PARAM);
	free(tmp);
	return (ret);
}

void	remove_single_quote(char **p, char **new_word)
{
	*p += 1;
	while (**p != SINGLE_QUOTE)
	{
		if (**p == '\0')
			assert_error("Unclosed single quote");
		if (**p == '$')
			g_signal.not_expand_flag = 1;
		append_char(new_word, **p);
		*p += 1;
	}
	*p += 1;
}
