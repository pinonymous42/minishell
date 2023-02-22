/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_ref.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 17:19:10 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/22 17:30:04 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*search_path(const char *filename)
{
	char	path[PATH_MAX];
	char	*value;
	char	*end;
	value = getenv("PATH");
	while (*value)
	{
		bzero(path, PATH_MAX);
		end = strchr(value, ':');
		if (end)
			strncpy(path, value, end - value);
		else
			strlcpy(path, value, PATH_MAX);
		strlcat(path, "/", PATH_MAX);
		strlcat(path, filename, PATH_MAX);
		if (access(path, X_OK) == 0)
		{
			char	*dup;
			dup = strdup(path);
			if (dup == NULL)
				function_error("strdup");
			return (dup);
		}
		if (end == NULL)
			return (NULL);
		value = end + 1;
	}
	return (NULL);
}

void	validate_access(const char *path, const char *filename)
{
	if (path == NULL)
		err_exit(filename, "command not found", 127);
	if (access(path, F_OK) < 0)
		err_exit(filename, "command not found", 127);
}

int	exec(char *argv[])
{
	extern char	**environ;
	const char	*path = argv[0];
	pid_t		pid;
	int			wstatus;
	pid = fork();
	if (pid < 0)
		function_error("fork");
	else if (pid == 0)
	{
		// child process
		if (strchr(path, '/') == NULL)
			path = search_path(path);
		validate_access(path, argv[0]);
		execve(path, argv, environ);
		function_error("execve");
	}
	else
	{
		// parent process
		wait(&wstatus);
		return (WEXITSTATUS(wstatus));
	}
}

void process_line(char *line, int *status)
{
    t_token *token;
	char	**argv;

    token = tokenize(line);
    if (token->kind == TOKEN_EOF)
        ;
    else if (syntax_error == true)
        *status = TOKENIZE_ERROR;
    else
    {
		expand(token);
		argv = token_list_to_argv(token);
		*status = exec(argv);
		free_argv(argv);
    }
    free_token(token);
}

int main(void)
{
    int status;
    char *line;

    rl_outstream = stderr; //ここ必要かあとで検討
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