/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_4_cd_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:23:51 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/13 16:50:42 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_new_pwd(t_environ *list, char *new_pwd_dup)
{
	while (list != NULL)
	{
		if (ft_strncmp(list->key, "PWD", ft_strlen("PWD")) == 0)
		{
			free(list->value);
			list->value = new_pwd_dup;
		}
		list = list->next;
	}
}

void	cd_builtin(t_info *info)
{
	char	*home;
	char	new_pwd[PATH_MAX];
	char	*dup;
	char	*error_pwd;

	if (info->argv[1] == NULL)
	{
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
		if (chdir(info->argv[1]) == -1)
			function_error("chdir");
	}
	if (getcwd(new_pwd, sizeof(new_pwd)) != NULL)
	{
		dup = ft_strdup(new_pwd);
		set_new_pwd(info->list, dup);
	}
	else
	{
		while (info->list != NULL)
		{
			if (ft_strncmp(info->list->key, "PWD", ft_strlen("PWD")) == 0)
			{
				error_pwd = ft_strjoin_with_free(info->list->value, "/.", 1);
				info->list->value = error_pwd;
				break ;
			}
			info->list = info->list->next;
		}
		perror("cd: error retrieving current directory: \
			getcwd: cannot access parent directories");
	}
}
