/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 13:19:43 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/02 15:46:14 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//strcmpあとで修正必要
int exec_builtin(char *exe_path, t_info *info)
{
	int		status;
    
	if (strcmp(exe_path, "pwd") == 0)
		status = builtin_pwd();
    else if (strcmp(exe_path, "export") == 0)
		status = builtin_exit(info);
    else if (strcmp(exe_path, "unset") == 0)
		status = builtin_unset();
	return (status);
}

bool	is_builtin(char *exe_path)
{
	const char		*cmd_name;
	char			*builtin_commands[] = {"pwd", "export", "unset"};
	unsigned int	i;

	cmd_name = exe_path;
	i = 0;
	while (i < 1)
	{
		if (strcmp(cmd_name, builtin_commands[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}