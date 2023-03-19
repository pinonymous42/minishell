/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_2_token_list_to_array.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:12:28 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/13 12:45:01 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ft_count(t_token *token)
{
	t_token *cur;
	size_t	size;

	size = 0;
	cur = token;
	while (cur != NULL && cur->kind != TOKEN_EOF)
    {
        size++;
        cur = cur->next;
    }
	return (size);
}

char	**token_list_to_array(t_token *token)
{
	char	**array;
	size_t	count;
	t_token *cur;
	size_t	i;

	count = 0;
    // printf(">%s<\n", token->word);
	count = ft_count(token);
    // printf("<%s>\n", token->word);
    array = ft_calloc(count + 1, sizeof(char *));
    if (array == NULL)
        function_error("calloc");
    cur = token;
    i = 0;
    while (cur != NULL && cur->kind != TOKEN_EOF && cur->word != NULL)
    {
        // printf("|%s|\n", cur->word);
        array[i] = ft_strdup(cur->word);
        if (array[i] == NULL)
            function_error("strdup");
        cur = cur->next;
        i++;
    }
    array[i] = NULL;
    return (array);
}
