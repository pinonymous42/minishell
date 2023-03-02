/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 23:07:46 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/02 15:39:31 by kohmatsu         ###   ########.fr       */
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

void    malloc_error_free(int **pipefd, int i)
{
    while (i >= 0)
    {
        free(pipefd[i]);
        i--;
    }
    free(pipefd);
    pipefd = NULL;
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

char    *make_exepath(char *path, char *command)
{
    char    *exe_path;
    char    *tmp;

    tmp = ft_strjoin(path, "/");
    exe_path = ft_strjoin(tmp, command);
    free(tmp);
    return (exe_path);
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

void    do_input(t_info *info, int i)
{
    char    **tmp;
    int     argv_index;
    int     tmp_index;
    
    argv_index = 0;
    tmp_index = 0;
    info->input_fd = open(info->argv[i + 1], O_RDONLY);
    if (info->input_fd == -1)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        err_exit(info->argv[i + 1], "command not found");
    }
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
    char    **tmp;
    int     argv_index;
    int     tmp_index;
        
    info->input_fd = open(".heredoc", O_RDONLY);
    if (info->input_fd == -1)
        function_error("open");

    argv_index = 0;
    tmp_index = 0;
    dup2(info->input_fd, STDIN);
    tmp = (char **)malloc(sizeof(char *) * ((info->argv_count - 2)));
    if (tmp == NULL)
        function_error("malloc4");
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
    char    **tmp;
    int     argv_index;
    int     tmp_index;
    
    printf("%s, %d\n", __FILE__, __LINE__);
    info->input_fd = open(".heredoc", O_RDONLY);
    if (info->input_fd == -1)
        function_error("open");

    argv_index = 0;
    tmp_index = 0;
    dup2(info->input_fd, STDIN);
    tmp = (char **)malloc(sizeof(char *) * ((info->argv_count - 2)));
    if (tmp == NULL)
        function_error("malloc4");
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
        // printf("|%s|\n", info->argv[i]);
        // printf(">%d<\n", info->heredoc_flag);
        if (ft_strncmp((info->argv)[i], "<<", 2) == 0 && info->heredoc_flag == 0)
        {
            // printf("%s, %d\n", __FILE__, __LINE__);
            if (heredoc_count == 1)
            {
                // printf("%s, %d\n", __FILE__, __LINE__);
                do_heredoc_one(info, i);
            }
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
            printf("%s, %d\n", __FILE__, __LINE__);
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

//////////////////////////////////////////////////////////////////ここから下は作成途中

void    make_info_argv(t_info *info, int end, int start)
{
    int pipe_index;
    char    **tmp;
    int argv_index;
    
    argv_index = 0;
    if (info->argv)
        safty_free(info->argv);
    pipe_index = start + 1;
    info->argv_count = end - start + count_splitable(info, pipe_index, end);
    info->argv = (char **)malloc(sizeof(char *) * (info->argv_count));
    if (info->argv == NULL)
        function_error("malloc1");
    while (pipe_index < end)
    {
        info->argv[argv_index] = ft_strdup(info->cmd[pipe_index]);
        if (!info->argv[argv_index])
            function_error("strdup");
        argv_index++;
        pipe_index++;
    }
    info->argv[argv_index] = NULL;
}

void    do_fd(t_info *info, int i)
{
    int j;

    j = 0;
    if (info->input_fd == 0)
    {
        if (i != 0)
            dup2(info->pipefd[i - 1][0], 0);
    }
    if (info->output_fd == 1)
    {
        if (i != info->pipe_count)
            dup2(info->pipefd[i][1], 1);  
    }
    while (j < info->pipe_count)
    {
        close(info->pipefd[j][0]);
        close(info->pipefd[j][1]);
        j++;
    }
}

void    multiple_pipes(t_info *info)
{
    int     i;
    pid_t   pid;
    char    *exe_path;
    int index;
    int wstatus;

    i = 0;
    exe_path = NULL;
    index = 0;
    while (i < info->pipe_count + 1)
    {
        pid = fork();
        if (pid == -1)
            function_error("fork");
        else if (pid == 0)
        {
            set_signal_child();
            make_info_argv(info, info->pipe_place[i + 1], info->pipe_place[i]);
            check_redirect(info);
            do_fd(info, i);
            
            exe_path = info->argv[0];
            while (access(exe_path, X_OK))
            {
                if (ft_strchr(info->argv[0], '/') == NULL)
                    exe_path = make_exepath((info->path)[index], (info->argv)[0]);
                else
                    exe_path = info->argv[0];
                index++;
            }
            if (exe_path == NULL)
            {
                // printf("%s, %d\n", __FILE__, __LINE__);
                err_exit(info->argv[0], "command not found");
            }
            if (access(exe_path, X_OK) < 0)
            {
                // printf("%s, %d\n", __FILE__, __LINE__);
                err_exit(info->argv[0], "command not found");
            }
            execve(exe_path, info->argv, list_to_array(info->list));
            function_error("execve");
        }
        else
        {
            set_signal_parent();
            if (i > 0)
            {
                close(info->pipefd[i - 1][0]);
                close(info->pipefd[i - 1][1]);
            }
        }
        i++;
    }
    i = 0;
    while (i < info->pipe_count + 1)
    {
        wait(&wstatus);
        i++;
    }
    if (wstatus == SIGINT)
        write(1, "\n", 1);
    else if (wstatus == SIGQUIT)
        write(1, "QUIT: 3\n", 8);
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

    ret = (int *)malloc(sizeof(int) * (info->pipe_count + 2));
    if (ret == NULL)
        function_error("malloc7");
    ret[0] = -1;
    ret[info->pipe_count + 1] = info->argc;
    i = 0;
    j = 1;
    while (i < info->argc)
    {
        if (*(info->cmd[i]) == '|' && ft_strlen(info->cmd[i]) == 1)
            ret[j++] = i;
        i++;
    }

    //pipefd作る
    info->pipefd = (int **)malloc(sizeof(int *) * (info->pipe_count));
    if (info->pipefd == NULL)
        function_error("malloc");
    i = 0;
    while (i < info->pipe_count)
    {
        info->pipefd[i] = (int *)malloc(sizeof(int) * 2);
        if (info->pipefd[i] == NULL)
        {
            malloc_error_free(info->pipefd, i);
            function_error("malloc");
        }
        i++;
    }
    i = 0;
    while (i < info->pipe_count)
        pipe(info->pipefd[i++]);
    return (ret);
}

void    info_init(t_info *info, int argc, char **argv, t_environ *list)
{
    info->input_fd = 0;
    info->output_fd = 1;
    info->argc = argc;//cmdの数  
    info->argv = NULL;   
    info->cmd = argv;
    info->pipe_count = count_pipe(info);
    info->path = make_path_list(list);
    info->list = list;
    // printf("%s, %d\n", __FILE__, __LINE__);
    info->pipefd = NULL;
    info->pipe_place = place_pipe(info);
    info->heredoc_flag = 0;
    // printf("%s, %d\n", __FILE__, __LINE__);
    // int i = 0;
    // while (i < info->pipe_count + 2)
    // {
    //     printf("%d\n", info->pipe_place[i]);
    //     i++;
    // }
    // exit(1);
    // info->heredoc_flag = 0;
}

/*---------------------------------------------------------------*/

void    finish(t_info *info)
{
    int i;

    i = 0;
    safty_free(info->path);
    safty_free(info->cmd);
    free(info->pipe_place);
    while (i < info->pipe_count)
    {
        free(info->pipefd[i]);
        i++;
    }
    free(info->pipefd);
    info->pipefd = NULL;
}


void pipex(int argc, char **argv, t_environ *list)
{
    t_info  info;
    
    // printf("%s, %d\n", __FILE__, __LINE__);
    info_init(&info, argc, argv, list);
    multiple_pipes(&info);
    finish(&info);
}