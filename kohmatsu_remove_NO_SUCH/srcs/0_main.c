/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 17:19:10 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/13 13:03:06 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void process_line(char *line, t_environ *list)
{
    t_token *token;
	char	**array;
	int		argc;
	// bool	syntax_error;

	// syntax_error = false;
    token = tokenize(line);
    
    if (g_signal.status == 258 && g_signal.other_code == TRUE)
    {
        free_token(token);
        return ;
    }
    
    g_signal.pipe_count = count_pipe(token);
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
        // printf("%s, %d\n", __FILE__, __LINE__);
		array = expand(token, list);
        //printf("%s, %d\n", __FILE__, __LINE__);
        // int i = 0;
        // while (array[i])
        // {
        //     printf("%s\n", array[i]);
        //     i++;
        // }
        // exit(1);
		// array = token_list_to_array(token);
        // printf("%s, %d\n", __FILE__, __LINE__);
		argc = count_argv(array);
        // printf("%s, %d\n", __FILE__, __LINE__);
        if (g_signal.other_code == FALSE)
        {
            // printf("%s, %d\n", __FILE__, __LINE__);
		    pipex(argc, array, list);
            // exit(1);
        }
        // printf("%s, %d\n", __FILE__, __LINE__);
		// free_array(array);
        // printf("%s, %d\n", __FILE__, __LINE__);
    }
    unlink("./.heredoc");
    // printf("%s, %d\n", __FILE__, __LINE__);
    free_token(token);
}

int main(int argc, char **argv, char **envp)
{
    char *line;
    t_environ *list;
    
    (void)argc;
    (void)argv;
    list = make_environ(envp);
    // g_signal.status = 0;
    g_signal.other_code = FALSE;
    while (1)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        //list = make_environ(envp);
        if (g_signal.other_code == FALSE)
            g_signal.status = 0;
        g_signal.other_code = FALSE;
        g_signal.input_fd = dup(0);
        g_signal.output_fd = dup(1);
        g_signal.do_split = 0;
        g_signal.pipe_count = 0;
	    g_signal.not_expand_flag = 0;
        set_signal();
        // printf("|%s|\n", line);
        // printf("%s, %d\n", __FILE__, __LINE__);
        line = readline("minishell$ ");
        // printf("<%s>\n", line);
        if (line == NULL)
            break;
        if (*line != '\0')
            add_history(line);
        process_line(line, list);
        free(line);
    }
    write(1, "exit\n", 5);
    free_list(list);
    return (0);
}