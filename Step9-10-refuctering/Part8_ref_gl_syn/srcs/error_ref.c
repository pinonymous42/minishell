/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_ref.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:33:36 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/23 18:53:30 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    function_error(char *function_name)
{
    printf("function error: %s\n", function_name);
    exit(EXIT_FAILURE);
}

//exitしない
void    tokenize_error(char *message, char **rest, char *line, bool *syntax_error)
{
    syntax_error = true;
    printf("minishell: syntax error near %s\n", message);
    while (*line)
		line++;
	*rest = line;
}

void	assert_error(const char *msg)
{
	dprintf(STDERR_FILENO, "Assert Error: %s\n", msg);
	exit(255);
}

void	err_exit(const char *location, const char *msg, int status)
{
	dprintf(STDERR_FILENO, "minishell: %s: %s\n", location, msg);
	exit(status);
}