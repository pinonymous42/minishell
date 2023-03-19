/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 16:17:01 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/25 23:17:58 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/step_1_4.h"

// void end(void)__attribute__((destructor));
// void end(void)
// {
//     system("leaks -q minishell");
// }

char *search_path(const char *filename)
{
    char path[PATH_MAX];
    char *value;
    char *end;

    //環境変数pathの中に入っている値をvalueに入れる
    //value：/bin:/usr/bin:/path/to/some/dir
    value = getenv("PATH");
    while (*value)
    {
        // /bin:/usr/bin
		//     ^
		//     end
        bzero(path, PATH_MAX);
        //":"の場所を特定
        end = strchr(value, ':');
        if (end != NULL) //:が見つかった場合、valueからendまでの文字列をpathにコピーした
            strncpy(path, value, end - value);
        else //:が見つからなかった場合、valueをpathにコピーした
            strlcpy(path, value, PATH_MAX);
        //「strlcat」関数を使用して、「path」に「/」と「filename」を連結しています。
        //「strlcat」関数は、「strcat」関数と似ていますが、結合する文字列の長さが最大値を超えないように制限されます。
        //最初の「strlcat」関数では、「/」が「path」に追加されます。
        strlcat(path, "/", PATH_MAX);
        //次の「strlcat」関数では、「filename」が「path」に追加されます。
        strlcat(path, filename, PATH_MAX);
        //「value」と「filename」から完全なパスを生成することを目的としている

        //そうして結合したパスが実行可能なパスなのかどうかは、accessを使って判定することができます。
        if (access(path, X_OK) == 0)
        {
            char *dup;

            dup = strdup(path);
            if (dup == NULL)
                fatal_error("strdup");
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
		fatal_error("fork");
	else if (pid == 0)
	{
		// child process
		if (strchr(path, '/') == NULL)
			path = search_path(path);
		validate_access(path, argv[0]);
		execve(path, argv, environ);
		fatal_error("execve");
	}
	else
    {
		// parent process
		wait(&wstatus);
		return (WEXITSTATUS(wstatus));
	}
}

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
void    interpret(char *line, int *stat_loc, t_environ *list)
{
    t_token	*token;
	char	**argv;
    t_node  *node;
    int     argc;

	token = tokenize(line);
    // printf("%s, %d\n", __FILE__, __LINE__);
    // while (token != NULL)
    // {
    //     printf("%s\n", token->word);
    //     token = token->next;
    // }
    // exit(1);
    if (token->kind == TK_EOF)
        ;
    else if (syntax_error)
        *stat_loc = ERROR_TOKENIZE;
    else
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        node = parse(token);
        // printf("%s, %d\n", __FILE__, __LINE__);
        //  while (node->args != NULL)
        // {
        //     printf("-----------------------------\n");
        //     printf("%s\n", node->args->word);
        //     node->args = node->args->next;
        // }
        // exit(1);
        expand(node, *stat_loc, list);
        // printf("%s, %d\n", __FILE__, __LINE__);
        // while (node->args != NULL)
        // {
        //     printf("-----------------------------\n");
        //     printf("%s\n", node->args->word);
        //     node->args = node->args->next;
        // }
        // exit(1);
        // printf("%s, %d\n", __FILE__, __LINE__);
        argv = token_list_to_argv(node->args);
        // printf("%s, %d\n", __FILE__, __LINE__);
        // while(*argv != NULL)
        // {
        //     printf("-----------------------\n");
        //     printf("%s\n", *argv);
        //     argv++;
        // }
        // exit(1);
        argc = count_argv(argv);
        // printf("%s, %d\n", __FILE__, __LINE__);
        *stat_loc = pipex(argc, argv, list);
        // printf("%s, %d\n", __FILE__, __LINE__);
        // printf("%d\n", *stat_loc);
        free_argv(argv);
        // printf("%s, %d\n", __FILE__, __LINE__);
        free_node(node);
        // printf("%s, %d\n", __FILE__, __LINE__);
    }
	free_all_token(token);
    // printf("%s, %d\n", __FILE__, __LINE__);
}

int main(int argc, char **argv, char **envp)
{
    int     status;
    char    *line;
    t_environ *list;
    
    list = make_environ(envp);
    // while (list != NULL)
    // {
    //     printf("%s=%s\n", list->key, list->value);
    //     list = list->next;
    // }
    // exit(1);
    // rl_outstream = stderr;
    status = 0;
    g_signal = 0;
    while (1)
    {
        set_signal();
        line = readline("minishell$ ");
        if (line == NULL)
            break;
        if (*line)
            add_history(line);
        interpret(line, &status, list);
        free(line);
        // exit(1);
    }
    exit(status);
}