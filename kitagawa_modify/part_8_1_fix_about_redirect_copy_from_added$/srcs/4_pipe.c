/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_pipe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 23:07:46 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/11 13:23:27 by yokitaga         ###   ########.fr       */
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
    // t_environ *tmp;
    
    // tmp = list;
    // while(list != NULL)
    // {
    //     printf("KEY:%s/", list->key);
    //     printf("VALUE:%s\n", list->value);
    //     list = list->next;
    // }
    // list = tmp;
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
        tmp = ft_strjoin(list->key, "=");
        ret[i] = ft_strjoin(tmp, list->value);
        free(tmp);
        list = list->next;
        i++;
    }
    
    // i = 0;
    // while (ret[i] != NULL) {
    //     ft_putendl_fd(ret[i], 1);
    //     i++;
    // }
    
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
    int i;

    count = 0;
    while (start < end)
    {
        if (ft_strchr(info->cmd[start], ' '))
        {
            i = 0;
            while (info->cmd[start][i])
            {
                if (info->cmd[start][i] == ' ')
                    count++;
                i++;
            }
        }
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
        //printf("%s, %d\n", __FILE__, __LINE__);
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
    int i;

    count = 0;
    i = 0;
    while (argv[i])
    {
        if (ft_strncmp(argv[i], "<<", 2) == 0)
            count++;
        i++;
    }
    return (count);
}

int    check_redirect(t_info *info)
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
            if (access(info->argv[i + 1], F_OK) == -1)
            {
                file_not_found(info->argv[i + 1]);
                return (1);
            }
            // printf("%s, %d\n", __FILE__, __LINE__);
            do_input(info, i);
            continue;
        }
        else if (*(info->argv)[i] == '>')
        {
            //printf("%s, %d\n", __FILE__, __LINE__);
            if (ft_strchr(info->argv[i + 1],'$') != NULL)//$が含まれている
            {
                 ambiguous_redirect(info->argv[i + 1]);
                 return (1);
            }
            do_output(info, i);
            continue;
        }
        i++;
    }
    return (0);
}

int check_builtin(t_info *info, char *argv)
{
    if (ft_strncmp(argv, "cd", 2) == 0)
        return (1);
    if (ft_strncmp(argv, "echo", 4) == 0)
        return (1);
    if (ft_strncmp(argv, "env", 3) == 0)
        return (1);
    if (ft_strncmp(argv, "exit", 4) == 0)
        return (1);
    if (ft_strncmp(argv, "export", 6) == 0)
        return (1);
    if (ft_strncmp(argv, "pwd", 3) == 0)
        return (1);
    if  (ft_strncmp(argv, "unset", 5) == 0)
        return (1);
    return (0);
}

void    make_info_argv(t_info *info, int end, int start)
{
    int pipe_index;
    char    **tmp;
    int argv_index;
    int split_count;
    
    // printf("start: %d, end: %d\n", start, end);
    argv_index = 0;
    if (info->argv)
        safty_free(info->argv);
    pipe_index = start + 1;//pipeの次のindex
    info->argv_count = end - start + count_splitable(info, pipe_index, end);
    // printf("|count: %d|\n", info->argv_count);
    info->argv = (char **)malloc(sizeof(char *) * (info->argv_count));
    if (info->argv == NULL)
        function_error("malloc1");
    while (pipe_index < end)
    {
            // printf("|%d|\n", pipe_index);
        if (ft_strchr(info->cmd[pipe_index], ' ') && g_signal.do_split)//スペースが含まれている
        {
            // printf("%s, %d\n", __FILE__, __LINE__);
            // printf(">%s<\n", info->cmd[pipe_index]);
            split_count = 0;
            tmp = ft_split(info->cmd[pipe_index], ' ');
            while (split_count < count_splitable(info, pipe_index, end) + 1)
            {
                info->argv[argv_index] = ft_strdup(tmp[split_count]);
                split_count++;
                argv_index++;
            }
            // printf("%s, %d\n", __FILE__, __LINE__);
            safty_free(tmp);
            pipe_index++;
            // printf("%s, %d\n", __FILE__, __LINE__);
        }
        // printf("{%s, %d}\n", info->cmd[pipe_index], pipe_index);
        else
        {
            // printf("%s, %d\n", __FILE__, __LINE__);
            // printf("|%s|\n", info->cmd[pipe_index]);
            // printf("pipe_index: %d, end: %d\n", pipe_index, end);
            // printf("%s, %d\n", __FILE__, __LINE__);
            // printf(">%s<\n", info->cmd[pipe_index]);
            info->argv[argv_index] = ft_strdup(info->cmd[pipe_index]);
            if (!info->argv[argv_index])
                function_error("strdup");
            argv_index++;
            pipe_index++;
        }
    }
    info->argv[argv_index] = NULL;
        // int i = 0;
        // while (info->argv[i])
        // {
        //     printf("|%s|\n", info->argv[i]);
        //     i++;
        // }
        // printf("------------------------\n");
    // printf("%s, %d\n", __FILE__, __LINE__);
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
        if (i != g_signal.pipe_count)
            dup2(info->pipefd[i][1], 1);  
    }
    while (j < g_signal.pipe_count)
    {
        close(info->pipefd[j][0]);
        close(info->pipefd[j][1]);
        j++;
    }
}

void    multiple_pipes(t_info *info, t_environ *list)
{
    int     i;
    pid_t   pid;
    char    *exe_path;
    int index;
    int wstatus;

    i = 0;
    exe_path = NULL;
    index = 0;
    while (i < g_signal.pipe_count + 1)
    {
        pid = fork();
        if (pid == -1)
            function_error("fork");
        else if (pid == 0)
        {
            set_signal_child();
            // printf("{%d, %d}\n", info->pipe_place[i + 1], info->pipe_place[i]);
            make_info_argv(info, info->pipe_place[i + 1], info->pipe_place[i]);
            // int x = 0;
            // printf("----------------------\n");
            // while (info->argv[x])
            // {
            //     printf("|%s|\n", info->argv[x]);
            //     x++;
            // }
            //printf("%s, %d\n", __FILE__, __LINE__);
            if (check_redirect(info) == 1)
                exit(1);
                // printf("%s, %d\n", __FILE__, __LINE__);
            do_fd(info, i);
            // printf("%s, %d\n", __FILE__, __LINE__);
            
            //以下はbash-3.2$ A= B= echo C=c
            //C=c
            //などの場合に対応するための処理
            int j;
            j = 0;
            while (info->argv[j] != NULL)
            {
                //info->argv[j]に=が含まれている場合はj++で次に進む
                //=が含まれていない場合は抜ける
                //printf("%d\n", j);
                if (ft_strchr_index(info->argv[j], '=') == -1)//=が含まれていない場合
                    break;
                j++;
            }
            
            if (check_builtin(info, info->cmd[info->pipe_place[i] + 1]) && i > g_signal.pipe_count - 1 && check_builtin(info, info->cmd[info->pipe_place[i - 1] + 1]))
            {
                // printf("%s, %d\n", __FILE__, __LINE__);
                // printf("|%s|\n", info->argv[0]);
                if (ft_strncmp(info->cmd[info->pipe_place[i] + 1], "cd", 2) == 0)
                    cd_builtin(info);
                else if (ft_strncmp(info->cmd[info->pipe_place[i] + 1], "echo", 4) == 0)
                    echo_builtin(info, i);
                else if (ft_strncmp(info->cmd[info->pipe_place[i] + 1], "exit", 4) == 0)
                    env_builtin(info);
                else if (ft_strncmp(info->cmd[info->pipe_place[i] + 1], "unset", 5) == 0)
                    unset_builtin(info, list);
                else if (ft_strncmp(info->cmd[info->pipe_place[i] + 1], "env", 3) == 0)
                    env_builtin(info);
                else if (ft_strncmp(info->cmd[info->pipe_place[i] + 1], "export", 6) == 0)
                    export_builtin(info, list);
                else if (ft_strncmp(info->cmd[info->pipe_place[i] + 1], "pwd", 3) == 0)
                    pwd_builtin(info);
                exit(0);
            }
            else if (check_builtin(info, info->cmd[info->pipe_place[i] + 1]))
            {
                printf("%s, %d\n", __FILE__, __LINE__);
                if (ft_strncmp(info->cmd[info->pipe_place[i] + 1], "env", 3) == 0)
                    env_builtin(info);
                else if (ft_strncmp(info->cmd[info->pipe_place[i] + 1], "export", 6) == 0)
                    export_builtin(info, list);
                else if (ft_strncmp(info->cmd[info->pipe_place[i] + 1], "pwd", 3) == 0)
                    pwd_builtin(info);
                exit(0);
            }
            else
            {
                //printf("%s, %d\n", __FILE__, __LINE__);
                if (access(exe_path, X_OK) == 0 && ft_strncmp(exe_path, "./", 2))
                {
                    // printf("%s, %d\n", __FILE__, __LINE__);
                    err_exit(info->argv[0], "command not found");
                }
                while ((info->path)[index] != NULL && access(exe_path, X_OK))
                {
                    if (ft_strchr(info->argv[0], '/') == NULL)
                        exe_path = make_exepath((info->path)[index], (info->argv)[0]);
                    else
                        exe_path = info->argv[0];
                    index++;
                }
                // printf("|%s|\n", exe_path);
                // if (exe_path == NULL)
                // {
                //     err_exit(info->argv[0], "command not found");
                // }
                // if (access(exe_path, X_OK) < 0)
                // {
                //     err_exit(info->argv[0], "command not found");
                // }
                
                if (execve(exe_path, info->argv, list_to_array(info->list)) == -1)
                {
                    //prinf("%d\n", ft_strchr(info->argv[0], '='));
                    if (ft_strchr_index(info->argv[j], '=') == -1)
                        command_not_found(info->argv[j]);
                    else
                        exit(0);
                    //err_exit(info->argv[0], "command not found");
                    //err_exit(info->argv[0], info->argv[0]);
                    // printf("%s, %d\n", __FILE__, __LINE__);
                    //exit(127);
                }
                // function_error("execve");
            }
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
    while (i < g_signal.pipe_count + 1)
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

int count_pipe(t_token *token)
{
    int count;
    int i;

    i = 0;
    count = 0;
    while (token->kind != TOKEN_EOF)
    {
        if (token->word[0] == '|' && ft_strlen(token->word) == 1)
            count++;
        token = token->next;
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

    // printf("?%s?\n", info->cmd[0]);
    ret = (int *)malloc(sizeof(int) * (g_signal.pipe_count + 2));
    if (ret == NULL)
        function_error("malloc7");
    ret[0] = -1;
    ret[g_signal.pipe_count + 1] = info->argc;
    // printf("pipe_place_end: %d\n", ret[g_signal.pipe_count + 1]);
    i = 0;
    j = 1;
    while (i < info->argc)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        // printf("i: %d, info->cmd: %s\n", i, info->cmd[i]);
        if (info->cmd[i][0] == '|' && ft_strlen(info->cmd[i]) == 1)
        {
            ret[j++] = i;
        }
        i++;
    }
    // printf("pipe_place_end: %d\n", ret[g_signal.pipe_count + 1]);

    //pipefd作る
    info->pipefd = (int **)malloc(sizeof(int *) * (g_signal.pipe_count));
    if (info->pipefd == NULL)
        function_error("malloc");
    i = 0;
    while (i < g_signal.pipe_count)
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
    while (i < g_signal.pipe_count)
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
    info->path = make_path_list(list);
    // if (info->updata_list == TRUE)
    //     ;
    // else
    info->list = list;
    // printf("%s, %d\n", __FILE__, __LINE__);
    info->pipefd = NULL;
    info->pipe_place = place_pipe(info);
    info->heredoc_flag = 0;
    // printf("|%s|\n", info->path[0]);
    // exit(1);
    // printf("%s, %d\n", __FILE__, __LINE__);
    // int i = 0;
    // while (i < g_signal.pipe_count + 2)
    // {
    //     printf("%d\n", info->pipe_place[i]);
    //     i++;
    // }
    // exit(1);
    // info->heredoc_flag = 0;
}

/*---------------------------------------------------------------*/

void    ft_environ_list_clear(t_environ **list)
{
    t_environ *tmp;

    if (list)
    {
        while (*list)
        {
            tmp = (*list)->next;
            free(*list);
            *list = tmp;
        }
        list = NULL;
    }
}

void    finish(t_info *info)
{
    int i;

    i = 0;
    if (info->path)
        safty_free(info->path);
    if (info->cmd)
        safty_free(info->cmd);
    free(info->pipe_place);
    if (info->pipefd)
    {
        while (i < g_signal.pipe_count)
        {
            free(info->pipefd[i]);
            i++;
        }
        free(info->pipefd);
        info->pipefd = NULL;
    }
    if (info->argv)
        safty_free(info->argv);
    // if (info->list)
    //      ft_environ_list_clear(&(info->list));
    dup2(g_signal.input_fd, 0);
    dup2(g_signal.output_fd, 1);
}

void pipex(int argc, char **argv, t_environ *list)
{
    t_info  info;
    
    info_init(&info, argc, argv, list);
    // printf("{%d}\n", g_signal.pipe_count);
    // printf("{%d}\n", info.pipe_place[0]);
    // printf("{%d}\n", info.pipe_place[1]);
    // printf("{%d}\n", info.pipe_place[2]);
    //printf("%s, %d\n", __FILE__, __LINE__);
    if (g_signal.pipe_count == 0 && check_builtin(&info, info.cmd[0]))
    {
        //printf("%s, %d\n", __FILE__, __LINE__);
        // int i = 0;
        // while(info.cmd[i])
        // {
        //     printf(">%s<\n", info.cmd[i]);
        //     i++;
        // }
        make_info_argv(&info, info.pipe_place[1], info.pipe_place[0]);
        if (check_redirect(&info) == 1)
        {
            g_signal.other_code = TRUE;
            g_signal.status = 1;
            finish(&info);
            return ;
        }
        // printf("-----------------------------------\n");
        // int i = 0;
        // while (info.argv[i])
        // {
        //     printf("%s\n", info.argv[i]);
        //     i++;
        // }
        // exit(1);
        
        // while (info.list->next != NULL)
        // {
        //     printf("%s\n", info.list->key);
        //     printf("%s\n", info.list->value);
        //     info.list = info.list->next;
        // }
        //printf("%s, %d\n", __FILE__, __LINE__);
        if (ft_strncmp(info.argv[0], "cd", 2) == 0)
            cd_builtin(&info);
        else if (ft_strncmp(info.argv[0], "echo", 4) == 0)
            echo_builtin(&info, 0);
        else if (ft_strncmp(info.argv[0], "env", 3) == 0)
            env_builtin(&info);
        else if (ft_strncmp(info.argv[0], "exit", 4) == 0)
            exit_builtin(&info);
        else if (ft_strncmp(info.argv[0], "pwd", 3) == 0)
            pwd_builtin(&info);
        else if (ft_strncmp(info.argv[0], "export", 6) == 0)
            export_builtin(&info, list);
        else if (ft_strncmp(info.argv[0], "unset", 5) == 0)
            unset_builtin(&info, list);
    }
    else
    {
        //printf("%s, %d\n", __FILE__, __LINE__);
        multiple_pipes(&info, list);
    }
    finish(&info);
}