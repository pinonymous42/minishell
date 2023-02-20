/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 16:17:01 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/20 18:04:00 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/step_9_10.h"

// void end(void)__attribute__((destructor));
// void end(void)
// {
//     system("leaks -q minishell");
// }

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
void    interpret(char *line, int *stat_loc)
{
    t_token	*token;
	char	**argv;
    t_node  *node;
    int     argc;

	token = tokenize(line);
    if (token->kind == TK_EOF)
        ;
    else if (syntax_error)
        *stat_loc = ERROR_TOKENIZE;
    else
    {
        node = parse(token);
        expand(node);
        argv = token_list_to_argv(node->args);
        argc = count_argv(argv);
        *stat_loc = pipex(argc, argv);
        free_argv(argv);
        free_node(node);
    }
	free_all_token(token);
}

int main(void)
{
    int     status;
    char    *line;
    
    rl_outstream = stderr;
    status = 0;
    while (1)
    {
        line = readline("minishell$ ");
        if (line == NULL)
            break;
        if (*line)
            add_history(line);
        interpret(line, &status);
        free(line);
    }
    exit(status);
}