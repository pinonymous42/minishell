/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_ref.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 17:19:10 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/23 18:56:37 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int count_argv(char **argv)
{
    int len;

    len = 0;
    while (*argv)
    {
        len++;
        argv++;
    }
    return (len);
}

void process_line(char *line, int *status)
{
    t_token *token;
	char	**array;
	int		argc;
	bool	syntax_error;

	syntax_error = false;
    token = tokenize(line, &syntax_error);
    if (token->kind == TOKEN_EOF)
        ;
    else if (syntax_error == true)
        *status = TOKENIZE_ERROR;
    else
    {
		expand(token);
		array = token_list_to_array(token);
		argc = count_argv(array);
		*status = pipex(argc, array);
		free_array(array);
    }
    free_token(token);
}

int main(void)
{
    int status;
    char *line;

    status = 0;
    while (1)
    {
        line = readline("minishell$ ");
        if (line == NULL)
            break;
        if (*line != '\0')
            add_history(line);
        process_line(line, &status);
        free(line);
    }
    exit(status);
}