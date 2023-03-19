/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_1_5_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 00:25:38 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/18 10:32:04 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*create_new_word_if_null(char *var, t_environ *list)
{
	char	*new_word;

	if (search_env(var, list) != NULL)
		new_word = ft_strdup(search_env(var, list));
	else
	{
		new_word = ft_strdup("$");
		new_word = ft_strjoin_with_free(new_word, var, FIRST_PARAM);
	}
	return (new_word);
}

void	create_new_word_if_not_null(char **new_word, char *var, t_environ *list)
{
	if (search_env(var, list) != NULL)
		*new_word = ft_strjoin_with_free(*new_word,
				search_env(var, list), FIRST_PARAM);
	else
	{
		*new_word = ft_strjoin_with_free(*new_word, "$", FIRST_PARAM);
		*new_word = ft_strjoin_with_free(*new_word, var, FIRST_PARAM);
	}
}

void	handle_empty_str(char **p, char **new_word)
{
	*p += 2;
	if (*new_word == NULL)
		*new_word = ft_strdup("");
}
