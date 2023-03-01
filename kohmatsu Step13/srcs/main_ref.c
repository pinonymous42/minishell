/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_ref.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 17:19:10 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/28 14:28:57 by kohmatsu         ###   ########.fr       */
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

void process_line(char *line, t_environ *list)
{
    t_token *token;
	char	**array;
	int		argc;
	// bool	syntax_error;

	// syntax_error = false;
    token = tokenize(line);
    // while (token)
    // {
    //     printf("%s\n", token->word);
    //     token = token->next;
    // }
    // exit(1);
    if (token->kind == TOKEN_EOF)
        ;
    // else if (syntax_error == true)
    //     *status = TOKENIZE_ERROR;
    else
    {
		array = expand(token, list);
        // while (token)
        // {
        //     printf("%s\n", token->word);
        //     token = token->next;
        // }
        // exit(1);
		// array = token_list_to_array(token);
		argc = count_argv(array);
        if (g_signal.other_code == FALSE)
		    pipex(argc, array, list);
		free_array(array);
    }
    unlink("./.heredoc");
    free_token(token);
}

int main(int argc, char **argv, char **envp)
{
    char *line;
    t_environ *list;

    // g_signal.status = 0;
    g_signal.other_code = FALSE;
    list = make_environ(envp);
    while (1)
    {
        if (g_signal.other_code == FALSE)
            g_signal.status = 0;
        g_signal.other_code = FALSE;
        g_signal.input_fd = dup(0);
        set_signal();
        line = readline("minishell$ ");
        if (line == NULL)
            break;
        if (*line != '\0')
            add_history(line);
        process_line(line, list);
        free(line);
    }
    write(1, "exit\n", 5);
    return (0);
}