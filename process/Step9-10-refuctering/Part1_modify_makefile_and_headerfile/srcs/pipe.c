/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 15:25:23 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/02/20 16:28:13 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/step_9_10.h"
// void end(void)__attribute__((destructor));
// void end(void)
// {
//     system("leaks -q pipex");
// }

void    safty_free(char **str)
{
    int i;

    i = 0;
    // printf("%s, %d\n", __FILE__, __LINE__);
    while (str[i])
    {
        free(str[i]);
        i++;
    }
    // printf("%s, %d\n", __FILE__, __LINE__);
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

    // while (*(info->cmd))
    // {
    //     printf("%s\n", *(info->cmd));
    //     info->cmd++;
    // }
    // exit(1);
    argv_index = 0;
    // printf("%s, %d\n", __FILE__, __LINE__);
    // printf("%d, %d\n", end, start);
    if (info->argv && (part == LEFT || part == MID))
        safty_free(info->argv);
    // printf("%s, %d\n", __FILE__, __LINE__);
    // if (strchr("<>", info->cmd[0][0]))
    pipe_index = start + 1;
    // else
    //     pipe_index = 0;
    // printf("%d, %d, %d\n", end, start, pipe_index);
    info->argv_count = end - start + count_splitable(info, pipe_index, end);
    // printf("|%d|\n", info->argv_count);
    // printf("%s, %d\n", __FILE__, __LINE__);
    info->argv = (char **)malloc(sizeof(char *) * (info->argv_count));
    if (info->argv == NULL)
        fatal_error("malloc1");
    while (pipe_index < end)
    {
        if (ft_strchr(info->cmd[pipe_index], ' '))
        {
            tmp = ft_split(info->cmd[pipe_index], ' ');
            while (*tmp)
            {
                info->argv[argv_index] = ft_strdup(*tmp);
                if (!info->argv[argv_index])
                    fatal_error("strdup");
                argv_index++;
                tmp++;
            }
            safty_free(tmp);
        }
        else
        {
            info->argv[argv_index] = ft_strdup(info->cmd[pipe_index]);
            if (!info->argv[argv_index])
                fatal_error("strdup");
            argv_index++;
        }
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
        fatal_error("open");
    dup2(info->input_fd, STDIN);
    tmp = (char **)malloc(sizeof(char *) * ((info->argv_count - 2)));
    if (tmp == NULL)
        fatal_error("malloc2");
    while (argv_index != i)
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            fatal_error("strdup");
        tmp_index++;
        argv_index++;
    }
    argv_index += 2;//<とfileを飛ばす
    while (info->argv[argv_index])
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            fatal_error("strdup");
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
        fatal_error("open");
    if (info->argv[i + 2] == NULL)
        dup2(info->output_fd, STDOUT);
    tmp = (char **)malloc(sizeof(char *) * ((info->argv_count - 2)));
    if (tmp == NULL)
        fatal_error("malloc3");
    while (argv_index != i)
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            fatal_error("strdup");
        tmp_index++;
        argv_index++;
    }
    argv_index += 2;//>とfileを飛ばす
    while (info->argv[argv_index])
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            fatal_error("strdup");
        tmp_index++;
        argv_index++;
    }
    safty_free(info->argv);
    info->argv = tmp;
}

void    do_heredoc(t_info *info, int i)
{
    char    *line;
    char    **tmp;
    int     argv_index;
    int     tmp_index;
        
    info->input_fd = open(".heredoc", (O_WRONLY | O_CREAT | O_TRUNC), 0644);
    if (info->input_fd == -1)
        fatal_error("open");
    while (1)
    {
        write(STDOUT, "> ", 2);
        line = get_next_line(STDIN);
        if (ft_strncmp(line, info->argv[i + 1], ft_strlen(info->argv[i + 1])) == 0)
            break ;
        write(info->input_fd, line, ft_strlen(line));
        free(line);
    }
    close(info->input_fd);
    info->input_fd = open(".heredoc", O_RDONLY);
    if (info->input_fd == -1)
        fatal_error("open");

    argv_index = 0;
    tmp_index = 0;
    dup2(info->input_fd, STDIN);
    tmp = (char **)malloc(sizeof(char *) * ((info->argv_count - 2)));
    // printf("%s, %d\n", __FILE__, __LINE__);
    if (tmp == NULL)
        fatal_error("malloc4");
    // printf("%s, %d\n", __FILE__, __LINE__);
    while (argv_index != i)
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            fatal_error("strdup");
        tmp_index++;
        argv_index++;
    }
    argv_index += 2;//<とfileを飛ばす
    while (info->argv[argv_index])
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            fatal_error("strdup");
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
        fatal_error("open");
    if (info->argv[i + 2] == NULL)
        dup2(info->output_fd, STDOUT);
    tmp = (char **)malloc(sizeof(char *) * ((info->argv_count - 2)));
    if (tmp == NULL)
        fatal_error("malloc5");
    while (argv_index != i)
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            fatal_error("strdup");
        tmp_index++;
        argv_index++;
    }
    argv_index += 2;//>とfileを飛ばす
    while (info->argv[argv_index])
    {
        tmp[tmp_index] = ft_strdup(info->argv[argv_index]);
        if (!tmp[tmp_index])
            fatal_error("strdup");
        tmp_index++;
        argv_index++;
    }
    safty_free(info->argv);
    info->argv = tmp;
}

void    check_redirect(t_info *info)
{
    int i;

    i = 0;
    while ((info->argv)[i] != NULL)
    {
        if (ft_strncmp((info->argv)[i], "<<", 2) == 0)
        {
            // printf("%s, %d\n", __FILE__, __LINE__);
            do_heredoc(info, i);
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
            // printf("%s, %d\n", __FILE__, __LINE__);
            do_output(info, i);
            continue;
        }
        // printf("|%d|\n", i);
        // printf("|%c|\n", *(info->argv)[i]);
        // int j = 0;
        // printf("-------------------------\n");
        // while ((info->argv)[j])
        // {
        //     printf("%s\n", (info->argv)[j]);
        //     j++;
        // }
        // exit(1);
        i++;
    } 
    // while (*(info->argv))
    // {
    //     printf("%s\n", *(info->argv));
    //     (info->argv)++;
    // }
    // exit(1);
}

void    dopipes(int i, t_info *info)
{
    pid_t   ret;
    int     pipefd[2];
    char    *exe_path;
    int     index;
    
    exe_path = NULL;
    index = 0;
    if (info->pipe_count == i)//一番左のコマンド
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        if (info->pipe_count != 0)
            make_info_argv(info, info->pipe_place[i - 1], -1, LEFT);
        else
            make_info_argv(info, info->argc, -1, LEFT);
        // printf("%s, %d\n", __FILE__, __LINE__);
        // while (*(info->argv))
        // {
        //     printf("%s\n", *(info->argv));
        //     info->argv++;
        // }
        // exit(1);
        /////////////////////////////////////////////////////////////////////////////////////実行コマンド作成
        check_redirect(info);
        // printf("%s, %d\n", __FILE__, __LINE__);
        while (access(exe_path, X_OK))
        {
            exe_path = make_exepath((info->path)[index], (info->argv)[0]);
            index++;
        }
        execve(exe_path, info->argv, info->envp);
    }
    else if (i == 0)//一番右のコマンド
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
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
            while (access(exe_path, X_OK))
            {
                exe_path = make_exepath((info->path)[index], (info->argv)[0]);
                index++;
            }
            execve(exe_path, info->argv, info->envp);
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
            while (access(exe_path, X_OK))
            {
                exe_path = make_exepath((info->path)[index], (info->argv)[0]);
                index++;
            }
            execve(exe_path, info->argv, info->envp);
        }
    }
   
}

char    **make_path_list(char **envp)
{
    char    *pointer_path;

    while (ft_strncmp(*envp, "PATH", 4))
        envp++;
    pointer_path = ft_strchr(*envp, '=');
    return (ft_split(pointer_path + 1, ':'));
}

int    multiple_pipes(t_info *info)
{
    pid_t   ret;
    int     wstatus;

    // printf("%s\n", info->argv[0]);

    if ((ret = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (ret == 0)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        dopipes(0, info);
    }
    else
    {
        wait(&wstatus);
            return (WEXITSTATUS(wstatus));
    }
    // while (*info->argv)
    //     {
    //         printf("%s\n", *info->argv);
    //         info->argv++;
    //     }
    // printf("%s\n", info->argv[0]);
    // printf("%s\n", info->argv[1]);
    // exit(1);
}

int count_pipe(t_info *info)
{
    int count;
    int i;

    i = 0;
    count = 0;
    // printf("%s, %d\n", __FILE__, __LINE__);
    while (info->cmd[i])
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        // printf("%c\n", info->cmd[i][0]);
        if (info->cmd[i][0] == '|' && ft_strlen(info->cmd[i]) == 1)
            count++;
        i++;
    }
    // printf("%s, %d\n", __FILE__, __LINE__);
    return (count);
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
            fatal_error("malloc6");
        ret[0] = -1;
        return (ret);
    }
    ret = (int *)malloc(sizeof(int) * (info->pipe_count));
    if (ret == NULL)
        fatal_error("malloc7");
    i = info->argc - 1;
    j = 0;
    // ret[j++] = 0;
    // ret[info->pipe_count + 1] = info->argc - 1;
    while (i >= 0)
    {
        if (*(info->cmd[i]) == '|' && ft_strlen(info->cmd[i]) == 1)
        // {
            ret[j++] = i;
            // printf("%d, %d\n", i, j);  
        // }
        i--;
    }
    // exit(1);
    return (ret);
}

void    info_init(t_info *info, int argc, char **argv)
{
    extern char **environ;

    info->input_fd = 0;
    info->output_fd = 0;
    // cmdcount(&argc, argv);
    info->argc = argc;//cmdの数
    // printf("|%d|\n", argc);  
    info->argv = NULL;   
    info->cmd = argv;
    info->pipe_count = count_pipe(info);
    info->path = make_path_list(environ);
    info->envp = environ;
    info->pipe_place = place_pipe(info);
    // int i = 0;
    // while (i < info->pipe_count)
    // {
    //     printf("%d\n", info->pipe_place[i]);
    //     i++;
    // }
    // exit(1);
}

void    finish(t_info *info)
{
    safty_free(info->path);
    free(info->pipe_place);
}

int pipex(int argc, char **argv)
{
    t_info  info;
    int     i;
    int			wstatus;
    
    info_init(&info, argc, argv);
    // printf("%d\n", info.pipe_count);
    // exit(1);
    // printf("%s, %d\n", __FILE__, __LINE__);
    wstatus = multiple_pipes(&info);
    // printf("%s, %d\n", __FILE__, __LINE__);
    // i = 0;
    // while (info.cmd[i])
    // {
    //     printf("%s\n", info.cmd[i]);
    //     i++;
    // }
    // printf("%d\n", info.argc);
    // exit(1);
    finish(&info);
    return (wstatus);
}