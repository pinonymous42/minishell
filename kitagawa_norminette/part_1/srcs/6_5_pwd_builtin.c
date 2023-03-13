/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_5_pwd_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 11:55:33 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/13 14:51:08 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	pwd_builtin(t_info *info)
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    }
    else
    {
        while (info->list != NULL)//PWDの値を取得
        {
            if (ft_strncmp(info->list->key,"PWD", ft_strlen("PWD")) == 0)
            {
                printf("%s\n", info->list->value);
                break;
            }
            info->list = info->list->next;
        }
    }
}

