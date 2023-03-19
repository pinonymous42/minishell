/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_4_cd_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:23:51 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/09 17:53:14 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
void    cd_builtin(t_info *info)
{
    char    *home;
    
    if (info->argv[1] == NULL)//引数がない場合はHOMEに移動
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        home = ft_strdup(search_env("HOME", info->list));
        if (home == NULL)
        {
            my_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
            g_signal.status = 1;
            g_signal.other_code = TRUE;
        }
        else if ((chdir(home)) == -1)//chdir() は、呼び出し元プロセスの現在の作業ディレクトリ (current working directory) を、 path に指定されたディレクトリに変更する。
            function_error("chdir");
        free(home);
    }
    else
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        if (chdir(info->argv[1]) == -1)//chdir() は、呼び出し元プロセスの現在の作業ディレクトリ (current working directory) を、 path に指定されたディレクトリに変更する。
            function_error("cd");
    }
}
*/

void set_new_pwd(t_environ *list, char *new_pwd_dup)
{
    while (list != NULL)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        if (ft_strncmp(list->key, "PWD", ft_strlen("PWD")) == 0)
        {
            free(list->value);
            // printf("%s, %d\n", __FILE__, __LINE__);
            list->value = new_pwd_dup;
            //printf("%s\n", list->value);
        }
        list = list->next;
    }
}

void    cd_builtin(t_info *info)
{
    char    *home;

    if (info->argv[1] == NULL)//引数がない場合はHOMEに移動
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        home = ft_strdup(search_env("HOME", info->list));
        if (home == NULL)
        {
            my_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
            g_signal.status = 1;
            g_signal.other_code = TRUE;
        }
        else if ((chdir(home)) == -1)//chdir() は、呼び出し元プロセスの現在の作業ディレクトリ (current working directory) を、 path に指定されたディレクトリに変更する。
            function_error("chdir");
        free(home);
    }
    else
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        if (chdir(info->argv[1]) == -1)//chdir() は、呼び出し元プロセスの現在の作業ディレクトリ (current working directory) を、 path に指定されたディレクトリに変更する。
            function_error("chdir");
    }
    //printf("info->argv[1] = %s\n", info->argv[1]);
    //old_pwd = search_env("PWD", info->list);
    //printf("old_pwd = %s\n", old_pwd);
    char    new_pwd[PATH_MAX];
    char    *dup;
    //printf("new_pwd = %s\n", getcwd(new_pwd, sizeof(new_pwd)));
    if (getcwd(new_pwd, sizeof(new_pwd)) != NULL) {
        //printf("%s\n", new_pwd);
        dup = ft_strdup(new_pwd);
        set_new_pwd(info->list, dup);
    }
    else
    {
        char *error_pwd;
        while (info->list != NULL)//PWDの値を取得
        {
            if (ft_strncmp(info->list->key,"PWD", ft_strlen("PWD")) == 0)
            {
                error_pwd = ft_strjoin_with_free(info->list->value, "/.", 1);
                info->list->value = error_pwd;
                break;
            }
            info->list = info->list->next;
        }
        perror("cd: error retrieving current directory: getcwd: cannot access parent directories");
    }
    //free(new_pwd);
}