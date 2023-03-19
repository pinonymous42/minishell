/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_token_list_to_array.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:12:28 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/22 17:08:33 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**tail_recursive(t_token *token, int nargs, char **argv)
{
	if (token == NULL || token->kind == TOKEN_EOF)
		return (argv);
	argv = reallocf(argv, (nargs + 2) * sizeof(char *));
	argv[nargs] = strdup(token->word);
	if (argv[nargs] == NULL)
		function_error("strdup");
	argv[nargs + 1] = NULL;
	return (tail_recursive(token->next, nargs + 1, argv));
}

char	**token_list_to_argv(t_token *token)
{
	char	**argv;

	argv = calloc(1, sizeof(char *));
	if (argv == NULL)
		function_error("calloc");
	return (tail_recursive(token, 0, argv));
}