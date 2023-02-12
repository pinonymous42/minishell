/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matsushimakouhei <matsushimakouhei@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 00:04:47 by matsushimak       #+#    #+#             */
/*   Updated: 2023/02/12 13:09:51 by matsushimak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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

void    dopipes(int i, t_info *info)
{
    pid_t   ret;
    int     pipefd[2];
    //[0]: read
    //[1]: write
    char    *exe_path;
    int     index;

    exe_path = NULL;
    index = 0;
    if (info->argc - 2 == i)//一番左のコマンドを実行
    {
        dup2(info->input_fd, STDIN);
        while (access(exe_path, X_OK))
        {
            exe_path = make_exepath((info->path)[index], (info->cmd)[info->argc - i - 1][0]);
            index++;
        }
        execve(exe_path, (info->cmd)[info->argc - i - 1], info->envp);
    }
    else if (i == 0)//一番右のコマンドを実行
    {
        dup2(info->output_fd, STDOUT);
        dopipes(i + 1, info);
    }
    else//真ん中ら辺のコマンド
    {
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
                exe_path = make_exepath((info->path)[index], (info->cmd)[info->argc - i - 1][0]);
                index++;
            }
            execve(exe_path, (info->cmd)[info->argc - i - 1], info->envp);
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

void    multiple_pipes(t_info *info)
{
    pid_t   ret;
    info->path = make_path_list(info->envp);
    if ((ret = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (ret == 0)
    {
        dopipes(0, info);
    }
    else
        wait(NULL);
}

void    make_info_cmd(t_info *info, char **argv, int argc)
{
    int i;
    int j;

    i = 1;
    j = 0;
    if (info->heredoc_flag)
    {
        argc = argc - 1;//inputfile用のmalloc
        info->argc = info->argc - 1;//dopipesでinfo->cmdの一番左のコマンドを最後に実行できるように
        i = 2;//本当はi = 3がいいんだけど、here_docの時はiも増えるから
    }
    info->cmd = (char ***)malloc(sizeof(char **) * argc);
    while (argv[i])
    {
        if (info->heredoc_flag && j == 0)
        {
            info->cmd[j] = (char **)malloc(sizeof(char *) * 2);
            info->cmd[j][0] = ft_strdup("input.txt");
            info->cmd[j][1] = NULL;
        }
        else
            info->cmd[j] = ft_split(argv[i], ' ');
        i++;
        j++;
    }
    info->cmd[j] = NULL;

    // i = 0;
    // while (info->cmd[i])
    // {
    //     j = 0;
    //     while(info->cmd[i][j])
    //     {
    //         printf("%s\n", info->cmd[i][j]);
    //         j++;
    //     }
    //     i++;
    // }
}

void    run_heredoc(t_info *info, char **argv, int argc)
{
    char    *buf;
    
    // (info->argc)--;
    info->output_fd = open(argv[argc - 1], (O_WRONLY | O_CREAT | O_APPEND), 0777);
    make_info_cmd(info, argv, argc);
    info->input_fd = open((info->cmd)[0][0], (O_WRONLY | O_CREAT), 0777);
    while (1)
    {
        write(STDOUT, "> ", 2);
        buf = get_next_line(STDIN);
        if (!ft_strncmp(argv[2], buf, ft_strlen(argv[2])))
            break ;
        write(info->input_fd, buf, ft_strlen(buf));
        free(buf);
    }
    free(buf);
    close(info->input_fd);
    info->input_fd = open((info->cmd)[0][0], O_RDONLY);
    multiple_pipes(info);
}

int main(int argc, char *argv[], char *envp[])
{
    t_info  info;
    pid_t ret;
    int     i;
    
    if (argc < 5)
    {
        perror("argument number");
        exit(1);
    }
    info.argc = argc - 1;
    info.envp = envp;
    if (!ft_strncmp(argv[1], "here_doc", 8))
    {
        info.heredoc_flag = 1;
        run_heredoc(&info, argv, argc);
    }
    else
    {
        info.heredoc_flag = 0;
        info.input_fd = open(argv[1], O_RDONLY);
        info.output_fd = open(argv[argc - 1], (O_WRONLY | O_CREAT | O_TRUNC), 0777);
        make_info_cmd(&info, argv, argc);
        multiple_pipes(&info);
    }
    safty_free(info.path);
    unlink(info.cmd[0][0]);
    i = 0;
    while (info.cmd[i])
    {
        safty_free(info.cmd[i]);
        i++;
    }
    free(info.cmd);
    close(info.input_fd);
    close(info.output_fd);
    return (0);
}
