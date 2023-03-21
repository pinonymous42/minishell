/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_4_cd_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:23:51 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/21 13:44:04 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	tokurei(t_environ *list, char *new_pwd_dup);
// {
// 	char	*key;
// 	char	*value;

// 	list->key = ft_strdup("OLDPWD");
// 	list->value = new_pwd_dup;
// 	key = ft_strdup("PWD");
// 	value = ft_strdup(new_pwd_dup);
// 	list->next = new_node(key, value);
// 	free(key);
// 	free(value);
// }

void	set_old_and_new_pwd(t_environ *list, char *new_pwd_dup)
{
	char	*key;
	char	*value;
	t_environ	*tmp;

	tmp = list;
	if (list != NULL && list->key == NULL && list->value == NULL)
	{
		//tokurei(list, new_pwd_dup);
		list->key = ft_strdup("OLDPWD");
		list->value = new_pwd_dup;
		key = ft_strdup("PWD");
		value = ft_strdup(new_pwd_dup);
		list->next = new_node(key, value);
		free(key);
		free(value);
	}
	else
	{
		while (tmp != NULL)
		{
			if (ft_strcmp(tmp->key, "OLDPWD") == 0)
			{
				free(tmp->value);
				tmp->value = ft_strdup(search_env("PWD", list));
				break ;
			}
			tmp = tmp->next;
		}
		tmp = list;
		while (tmp != NULL)
		{
			if (ft_strcmp(tmp->key, "PWD") == 0)
			{
				free(tmp->value);
				tmp->value = new_pwd_dup;
				break ;
			}
			tmp = tmp->next;
		}
	}
}

void	get_old_and_new_pwd(t_info *info)
{
	char	new_pwd[PATH_MAX];
	char	*dup;
	char	*error_pwd;

	if (getcwd(new_pwd, sizeof(new_pwd)) != NULL)
	{
		dup = ft_strdup(new_pwd);
		set_old_and_new_pwd(info->list, dup);
	}
	else
	{
		while (info->list != NULL)
		{
			if (ft_strcmp(info->list->key, "PWD") == 0)
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

void	do_no_argv(t_info *info)
{
	char	*home;

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

void	cd_minus(t_info *info)
{
	if (search_env("OLDPWD", info->list) == NULL)
	{
		my_dprintf(STDERR_FILENO, "minishell: cd: OLDPWD not set\n");
		g_signal.status = 1;
		g_signal.other_code = TRUE;
	}
	else
	{
		if (chdir(search_env("OLDPWD", info->list)) == -1)
			function_error("chdir");
	}
}

void	cd_builtin(t_info *info)
{
	if (info->argv[1] == NULL)
		do_no_argv(info);
	else
	{
		if (info->argv[1][0] == '-')
		{
			if (info->argv[1][1] == '\0')
				cd_minus(info);
			else if (info->argv[1][1] == '-')
				do_no_argv(info);
			else
				my_dprintf(STDERR_FILENO, "minishell: cd: -%s: invalid option\n",
					info->argv[1]);
		}
		else if (chdir(info->argv[1]) == -1)
		{
			my_dprintf(STDERR_FILENO, "minishell: cd:%s:\
No such file or directory\n", info->argv[1]);
			g_signal.status = 1;
			g_signal.other_code = TRUE;
		}
	}
	get_old_and_new_pwd(info);
}
