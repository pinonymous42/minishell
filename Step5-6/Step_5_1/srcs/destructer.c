/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 17:33:46 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/09 11:42:02 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/step_5_6.h"

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