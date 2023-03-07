/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:23:51 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/03 23:16:12 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    cd_builtin(t_info *info)
{
    char    *home;
    
    if (info->argv[1] == NULL)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        home = ft_strdup(search_env("HOME", info->list));
        if (home == NULL)
        {
            my_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
            g_signal.status = 1;
            g_signal.other_code = TRUE;
        }
        else if ((chdir(home)) == -1)
            function_error("chdir");
        free(home);
    }
    else
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        if (chdir(info->argv[1]) == -1)
            function_error("cd");
    }
}