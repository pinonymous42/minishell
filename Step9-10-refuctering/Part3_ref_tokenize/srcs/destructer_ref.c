/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructer_ref.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 01:40:47 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/22 17:08:59 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_token(t_token *token)
{
	if (token == NULL)
		return ;
	if (token->word)
		free(token->word);
	free_token(token->next);
	free(token);
}

void	free_argv(char **argv)
{
	int	i;

	if (argv == NULL)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}