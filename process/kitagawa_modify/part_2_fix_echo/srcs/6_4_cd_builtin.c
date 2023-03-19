/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_4_cd_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:23:51 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/08 19:35:45 by yokitaga         ###   ########.fr       */
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

void    cd_builtin(t_info *info)
{
    char    *home;
    char    *path;
    char    *old_pwd;
    char    *update_pwd;

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
    old_pwd = search_env("PWD", info->list);
    printf("old_pwd = %s\n", old_pwd);
    // new_pwd = update_pwd(info->argv[1], old_pwd);
    // set_new_pwd(info->list, new_pwd);
    // free(new_pwd);
}