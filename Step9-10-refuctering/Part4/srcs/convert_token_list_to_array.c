/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_token_list_to_array.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:12:28 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/22 18:27:21 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//refuctering必要

char	**tail_recursive(t_token *token, int nargs, char **array)
{
	if (token == NULL || token->kind == TOKEN_EOF)
		return (array);
	array = reallocf(array, (nargs + 2) * sizeof(char *));
	array[nargs] = ft_strdup(token->word);
	if (array[nargs] == NULL)
		function_error("strdup");
	array[nargs + 1] = NULL;
	return (tail_recursive(token->next, nargs + 1, array));
}

char	**token_list_to_array(t_token *token)
{
	char	**array;

	array = ft_calloc(1, sizeof(char *));
	if (array == NULL)
		function_error("calloc");
	return (tail_recursive(token, 0, array));
}