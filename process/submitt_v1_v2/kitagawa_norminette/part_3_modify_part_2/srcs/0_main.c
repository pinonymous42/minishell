/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 17:19:10 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/14 20:39:13 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void end(void)__attribute__((destructor));
// void end(void)
// {
//     system("leaks -q minishell");
// }

int	count_argv(char **argv)
{
	int	len;

	len = 0;
	while (*argv)
	{
		len++;
		argv++;
	}
	return (len);
}

void	process_line(char *line, t_environ *list)
{
	t_token	*token;
	char	**array;
	int		argc;

	token = tokenize(line);
	if (g_signal.status == 258 && g_signal.other_code == TRUE)
	{
		printf("%s, %d\n", __FILE__, __LINE__);
		free_token(token);
		return ;
	}
	g_signal.pipe_count = count_pipe(token);
	if (token->kind == TOKEN_EOF)
		;
	else
	{
		array = expand(token, list);
		argc = count_argv(array);
		if (g_signal.other_code == FALSE)
		{
			pipex(argc, array, list);
		}
	}
	unlink("./.heredoc");
	free_token(token);
}

void	init_g_signal(void)
{
	g_signal.other_code = FALSE;
	g_signal.input_fd = dup(0);
	g_signal.output_fd = dup(1);
	g_signal.do_split = 0;
	g_signal.pipe_count = 0;
	g_signal.not_expand_flag = 0;
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_environ	*list;

	(void)argc;
	(void)argv;
	list = make_environ(envp);
	g_signal.other_code = FALSE;
	while (1)
	{
		if (g_signal.other_code == FALSE)
			g_signal.status = 0;
		init_g_signal();
		set_signal();
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line != '\0')
			add_history(line);
		process_line(line, list);
		free(line);
	}
	write(1, "exit\n", 5);
	free_list(list);
	return (0);
}
