/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 17:33:46 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/14 18:57:27 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/step_9_10.h"

void	free_node(t_node *node)
{
	if (node == NULL)
		return ;
	free_all_token(node->args);
	free_node(node->next);
	free(node);
}

void free_all_token(t_token *token)
{
    if (token == NULL)
        return ;
    if (token->word != NULL)
        free(token->word);
    free_all_token(token->next);
    free(token);
}

void free_argv(char **argv)
{
    size_t  i;

    if (argv == NULL)
        return ;
    i = 0;
    while (argv[i] != NULL)
    {
        free(argv[i]);
        i++;
    }
    free(argv);
}