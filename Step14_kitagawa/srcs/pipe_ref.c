/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_ref.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 01:40:43 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/28 13:57:25 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// void end(void)__attribute__((destructor));
// void end(void)
// {
//     system("leaks -q pipex");
// }

void    safty_free(char **str)
{
    int i;

    i = 0;
    while (str[i])
    {
        free(str[i]);
        i++;
    }
    free(str);
    str = NULL;
}

char    *make_exepath(char *path, char *command)
{
    char    *exe_path;
    char    *tmp;

    tmp = ft_strjoin(path, "/");
    exe_path = ft_strjoin(tmp, command);
    free(tmp);
    return (exe_path);
}

char	*search_path(const char *filename)
{
	char	path[PATH_MAX];
	char	*value;
	char	*end;
	value = getenv("PATH");
	while (*value)
	{
        //bzero(path, PATH_MAX);
		ft_bzero(path, PATH_MAX);
		end = ft_strchr(value, ':');
		if (end)
			//strncpy(path, value, end - value);
            ft_strlcpy(path, value, end - value + 1);
		else
			ft_strlcpy(path, value, PATH_MAX);
		ft_strlcat(path, "/", PATH_MAX);
		ft_strlcat(path, filename, PATH_MAX);
		if (access(path, X_OK) == 0)
		{
			char	*dup;
			dup = ft_strdup(path);
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


int count_splitable(t_info *info, int start, int end)
{
    int count;

    count = 0;
    while (start < end)
    {
        if (ft_strchr(info->cmd[start], ' '))
            count++;
        start++;
    }
    return (count);
}

void    make_info_argv(t_info *info, int end, int start, int part)
{
    int pipe_index;
    char    **tmp;
    int argv_index;
    
    argv_index = 0;
    if (info->argv && (part == LEFT || part == MID))
        safty_free(info->argv);
    pipe_index = start + 1;
    info->argv_count = end - start + count_splitable(info, pipe_index, end);
    info->argv = (char **)malloc(sizeof(char *) * (info->argv_count));
    if (info->argv == NULL)
        function_error("malloc1");
    while (pipe_index < end)
    {
        /*
        if (ft_strchr(info->cmd[pipe_index], ' '))
        {
            tmp = ft_split(info->cmd[pipe_index], ' ');
            while (*tmp)
            {
                info->argv[argv_index] = ft_strdup(*tmp);
                if (!info->argv[argv_index])
                    function_error("strdup");
                argv_index++;
                tmp++;
            }
            safty_free(tmp);
        }
        */
        //else
        //{
        info->argv[argv_index] = ft_strdup(info->cmd[pipe_index]);
        if (!info->argv[argv_index])
            function_error("strdup");
        argv_index++;
        //}
        pipe_index++;
    }
    info->argv[argv_index] = NULL;
}

void    do_input(t_info *info, int i)
{
    char    **tmp;
    int     argv_index;
    int     tmp_index;
    
    argv_index = 0;
    tmp_index = 0;
    info->input_fd = open(info->argv[i + 1], O_RDONLY);
    if (info->input_fd == -1)
        err_exit(info->argv[i + 1], "command not found");
        // function_error("open");
    dup2(info->input_fd, STDIN);
    tmp = (char **)malloc(sizeof(char *) * ((info->argv_count - 2)));
    if (tmp == NULL)
        function_error("malloc2");
    while (argv_index != i)
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            function_error("strdup");
        tmp_index++;
        argv_index++;
    }
    argv_index += 2;//<とfileを飛ばす
    while (info->argv[argv_index])
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            function_error("strdup");
        tmp_index++;
        argv_index++;
    }
    safty_free(info->argv);
    info->argv = tmp;
}

void    do_output(t_info *info, int i)
{
    char    **tmp;
    int     argv_index;
    int     tmp_index;
    
    argv_index = 0;
    tmp_index = 0;
    info->output_fd = open(info->argv[i + 1], (O_WRONLY | O_CREAT | O_TRUNC), 0644);
    if (info->output_fd == -1)
        function_error("open");
    if (info->argv[i + 2] == NULL)
        dup2(info->output_fd, STDOUT);
    tmp = (char **)malloc(sizeof(char *) * ((info->argv_count - 2)));
    if (tmp == NULL)
        function_error("malloc3");
    while (argv_index != i)
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            function_error("strdup");
        tmp_index++;
        argv_index++;
    }
    argv_index += 2;//>とfileを飛ばす
    while (info->argv[argv_index])
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            function_error("strdup");
        tmp_index++;
        argv_index++;
    }
    safty_free(info->argv);
    info->argv = tmp;
}

void    do_heredoc_one(t_info *info, int i)
{
    // char    *line;
    char    **tmp;
    int     argv_index;
    int     tmp_index;
        
    // info->input_fd = open(".heredoc", (O_WRONLY | O_CREAT | O_TRUNC), 0644);
    // if (info->input_fd == -1)
    //     function_error("open");
    // while (1)
    // {
    //     write(STDOUT, "> ", 2);
    //     // heredoc_signal();
    //     line = get_next_line(STDIN);
    //     if (ft_strncmp(line, info->argv[i + 1], ft_strlen(info->argv[i + 1])) == 0)
    //         break ;
    //     write(info->input_fd, line, ft_strlen(line));
    //     free(line);
    // }
    // close(info->input_fd);
    info->input_fd = open(".heredoc", O_RDONLY);
    if (info->input_fd == -1)
        function_error("open");

    argv_index = 0;
    tmp_index = 0;
    dup2(info->input_fd, STDIN);
    tmp = (char **)malloc(sizeof(char *) * ((info->argv_count - 2)));
    // printf("%s, %d\n", __FILE__, __LINE__);
    if (tmp == NULL)
        function_error("malloc4");
    // printf("%s, %d\n", __FILE__, __LINE__);
    while (argv_index != i)
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            function_error("strdup");
        tmp_index++;
        argv_index++;
    }
    argv_index += 2;//<とfileを飛ばす
    while (info->argv[argv_index])
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            function_error("strdup");
        tmp_index++;
        argv_index++;
    }
    safty_free(info->argv);
    info->argv = tmp;
}

void    do_heredoc_not_one(t_info *info, int i)
{
    // char    *line;
    char    **tmp;
    int     argv_index;
    int     tmp_index;
    
    // if (ft_strncmp(info->argv[i + 4], "<<", 2) == 0)
    //     return ;
    // info->heredoc_flag = 1;
    // info->input_fd = open(".heredoc", (O_WRONLY | O_CREAT | O_TRUNC), 0644);
    // if (info->input_fd == -1)
    //     function_error("open");
    // // heredoc_signal();
    // while (1)
    // {
    //     write(STDOUT, "> ", 2);
    //     line = get_next_line(STDIN);
    //     if (ft_strncmp(line, info->argv[i + 1], ft_strlen(info->argv[i + 1])) == 0)
    //         break ;
    //     free(line);
    // }
    // while (1)
    // {
    //     write(STDOUT, "> ", 2);
    //     line = get_next_line(STDIN);
    //     if (ft_strncmp(line, info->argv[i + 3], ft_strlen(info->argv[i + 3])) == 0)
    //         break ;
    //     write(info->input_fd, line, ft_strlen(line));
    //     free(line);
    // }
    // close(info->input_fd);
    info->input_fd = open(".heredoc", O_RDONLY);
    if (info->input_fd == -1)
        function_error("open");

    argv_index = 0;
    tmp_index = 0;
    dup2(info->input_fd, STDIN);
    tmp = (char **)malloc(sizeof(char *) * ((info->argv_count - 2)));
    // printf("%s, %d\n", __FILE__, __LINE__);
    if (tmp == NULL)
        function_error("malloc4");
    // printf("%s, %d\n", __FILE__, __LINE__);
    while (argv_index != i)
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            function_error("strdup");
        tmp_index++;
        argv_index++;
    }
    argv_index += 2;//<とfileを飛ばす
    while (info->argv[argv_index])
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            function_error("strdup");
        tmp_index++;
        argv_index++;
    }
    safty_free(info->argv);
    info->argv = tmp;
}

void    do_append(t_info *info, int i)
{
    char    **tmp;
    int     argv_index;
    int     tmp_index;
    
    argv_index = 0;
    tmp_index = 0;
    info->output_fd = open(info->argv[i + 1], (O_WRONLY | O_CREAT | O_APPEND), 0644);
    if (info->output_fd == -1)
        function_error("open");
    if (info->argv[i + 2] == NULL)
        dup2(info->output_fd, STDOUT);
    tmp = (char **)malloc(sizeof(char *) * ((info->argv_count - 2)));
    if (tmp == NULL)
        function_error("malloc5");
    while (argv_index != i)
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            function_error("strdup");
        tmp_index++;
        argv_index++;
    }
    argv_index += 2;//>とfileを飛ばす
    while (info->argv[argv_index])
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            function_error("strdup");
        tmp_index++;
        argv_index++;
    }
    safty_free(info->argv);
    info->argv = tmp;
}

int count_heredoc(char **argv)
{
    int count;

    count = 0;
    while (*argv)
    {
        if (ft_strncmp(*argv, "<<", 2) == 0)
            count++;
        argv++;
    }
    return (count);
}

void    check_redirect(t_info *info)
{
    int i;
    int heredoc_count;

    i = 0;
    heredoc_count = count_heredoc(info->argv);
    while ((info->argv)[i] != NULL)
    {
        if (ft_strncmp((info->argv)[i], "<<", 2) == 0 && info->heredoc_flag == 0)
        {
            if (heredoc_count == 1)
                do_heredoc_one(info, i);
            else
                do_heredoc_not_one(info, i);
            continue;
        }
        else if (ft_strncmp((info->argv)[i], ">>", 2) == 0)
        {
            do_append(info, i);
            continue;
        }
        else if (*(info->argv)[i] == '<')
        {
            do_input(info, i);
            continue;
        }
        else if (*(info->argv)[i] == '>')
        {
            do_output(info, i);
            continue;
        }
        i++;
    }
}

void    dopipes(int i, t_info *info)
{
    pid_t   ret;
    int     pipefd[2];
    char    *exe_path;
    int     index;
    
    exe_path = NULL;
    index = 0;
    set_signal_child();
    if (info->pipe_count == i)//一番左のコマンド
    {
        if (info->pipe_count != 0)
            make_info_argv(info, info->pipe_place[i - 1], -1, LEFT);
        else
            make_info_argv(info, info->argc, -1, LEFT);
        /////////////////////////////////////////////////////////////////////////////////////実行コマンド作成
        check_redirect(info);
        exe_path = info->argv[0];
        if (ft_strchr(exe_path, '/') == NULL)
            exe_path = search_path(exe_path);
        /*
        while (access(exe_path, X_OK))
        {
            if (ft_strchr(info->argv[0], '/') == NULL)
                exe_path = make_exepath((info->path)[index], (info->argv)[0]);
            else
                exe_path = info->argv[0];
            index++;
        }
        */
        if (exe_path == NULL)
        {
            // printf("%s, %d\n", __FILE__, __LINE__);
            err_exit(info->argv[0], "command not found");
        }
        if (access(exe_path, X_OK) < 0)
            err_exit(info->argv[0], "command not found");
        execve(exe_path, info->argv, info->envp);
        function_error("execve");
    }
    else if (i == 0)//一番右のコマンド
    {
        make_info_argv(info, info->argc, info->pipe_place[0], RIGHT);
        /////////////////////////////////////////////////////////////////////////////////////実行コマンド作成
        check_redirect(info);
        pipe(pipefd);
        if ((ret = fork()) == -1)
        {
            perror("fork");
            exit(1);
        }
        if (ret == 0)
        {
            // close(1);
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT);
            close(pipefd[1]);
            dopipes(i + 1, info);
        }
        else
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN);
            close(pipefd[0]);
            exe_path = info->argv[0];
            if (ft_strchr(exe_path, '/') == NULL)
                exe_path = search_path(exe_path);
            /*
            while (access(exe_path, X_OK))
            {
                if (ft_strchr(info->argv[0], '/') == NULL)
                    exe_path = make_exepath((info->path)[index], (info->argv)[0]);
                else
                    exe_path = info->argv[0];
                index++;
            }
            */
            if (exe_path == NULL)
                err_exit(info->argv[0], "command not found");
            if (access(exe_path, X_OK) < 0)
                err_exit(info->argv[0], "command not found");
            execve(exe_path, info->argv, info->envp);
            function_error("execve");
        }
    }
    else//それ以外の場所
    {
        make_info_argv(info, info->pipe_place[i - 1], info->pipe_place[i], MID);
        /////////////////////////////////////////////////////////////////////////////////////実行コマンド作成
        check_redirect(info);
        pipe(pipefd);
        if ((ret = fork()) == -1)
        {
            perror("fork");
            exit(1);
        }
        if (ret == 0)
        {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT);
            close(pipefd[1]);
            dopipes(i + 1, info);
        }
        else
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN);
            close(pipefd[0]);
            exe_path = info->argv[0];
            if (ft_strchr(exe_path, '/') == NULL)
                exe_path = search_path(exe_path);
            /*
            while (access(exe_path, X_OK))
            {
                if (ft_strchr(info->argv[0], '/') == NULL)
                    exe_path = make_exepath((info->path)[index], (info->argv)[0]);
                else
                    exe_path = info->argv[0];
                index++;
            }
            */
            if (exe_path == NULL)
                err_exit(info->argv[0], "command not found");
            if (access(exe_path, X_OK) < 0)
                err_exit(info->argv[0], "command not found");
            execve(exe_path, info->argv, info->envp);
            function_error("execve");
        }
    }
}

void    multiple_pipes(t_info *info)
{
    pid_t   ret;
    int     wstatus;

    if ((ret = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (ret == 0) //child process
        dopipes(0, info);
    else
    {
        //parent process
        set_signal_parent();
        wait(&wstatus);
        // printf("{%d}\n", wstatus);
        if (wstatus == SIGINT)
            write(1, "\n", 1);
        else if (wstatus == SIGQUIT)
            write(1, "QUIT: 3\n", 8);
        // if (wstatus < 256)
        // {
        if (wstatus == SIGINT || wstatus == SIGQUIT)
        {
            g_signal.status = 128 + wstatus;
            g_signal.other_code = TRUE;
        }
        else
        {
            g_signal.status = WEXITSTATUS(wstatus);
            g_signal.other_code = TRUE;
        }
        // }
        // else
        // {
        //     printf("<%d>\n", wstatus);
        //     // g_signal.status = wstatus / 256;
        //     g_signal.status = WEXITSTATUS(wstatus);
        //     printf("%d\n", g_signal.status);
        // }
    }
}

/*-------------------------------------------------------------*/

int count_pipe(t_info *info)
{
    int count;
    int i;

    i = 0;
    count = 0;
    while (info->cmd[i])
    {
        if (info->cmd[i][0] == '|' && ft_strlen(info->cmd[i]) == 1)
            count++;
        i++;
    }
    return (count);
}

char    **make_path_list(t_environ *list)
{
    while (list != NULL)
    {
        if (ft_strncmp(list->key, "PATH", 4) == 0)
            return (ft_split(list->value, ':'));
        list = list->next;
    }
    // fatal_error("PATH");
    return (NULL);
}

int *place_pipe(t_info *info)
{
    int    *ret;
    int i;
    int j;

    if (info->pipe_count == 0)
    {
        ret = (int *)malloc(sizeof(int) * 1);
        if (ret == NULL)
            function_error("malloc6");
        ret[0] = -1;
        return (ret);
    }
    ret = (int *)malloc(sizeof(int) * (info->pipe_count));
    if (ret == NULL)
        function_error("malloc7");
    i = info->argc - 1;
    j = 0;
    while (i >= 0)
    {
        if (*(info->cmd[i]) == '|' && ft_strlen(info->cmd[i]) == 1)
            ret[j++] = i;
        i--;
    }
    return (ret);
}

int count_list_len(t_environ *list)
{
    int count;

    count = 0;
    while (list != NULL)
    {
        count++;
        list = list->next;
    }
    return (count);
}

char **list_to_array(t_environ *list)
{
    char **ret;
    char *tmp;
    int i;
    
    i = 0;
    ret = (char **)malloc(sizeof(char *) * (count_list_len(list) + 1));
    if (!ret)
        function_error("malloc");
    while (list != NULL)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        tmp = ft_strjoin(list->key, "=");
        ret[i] = ft_strjoin(tmp, list->value);
        free(tmp);
        list = list->next;
        i++;
    }
    ret[i] = NULL;
    return (ret);
}

void    info_init(t_info *info, int argc, char **argv, t_environ *list)
{
    info->input_fd = 0;
    info->output_fd = 0;
    info->argc = argc;//cmdの数  
    info->argv = NULL;   
    info->cmd = argv;
    info->pipe_count = count_pipe(info);
    info->path = make_path_list(list);
    info->envp = list_to_array(list);
    info->pipe_place = place_pipe(info);
    info->heredoc_flag = 0;
}

/*---------------------------------------------------------------*/

void    finish(t_info *info)
{
    safty_free(info->path);
    safty_free(info->envp);
    free(info->pipe_place);
    // close(info->input_fd);
    // close(info->output_fd);
}


void pipex(int argc, char **argv, t_environ *list)
{
    t_info  info;
    int     i;
    int		wstatus;
    
    info_init(&info, argc, argv, list);
    multiple_pipes(&info);
    finish(&info);
}