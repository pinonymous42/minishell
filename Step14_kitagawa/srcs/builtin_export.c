/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:34:51 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/02 16:34:40 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strchr_index(const char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

void	export_env(char *arg)
{
	char	*key;
	char	*value;
	int		i;

	i = ft_strchr_index(arg, '=');
	if (i == -1)
		err_exit()
	else
	{
		key = ft_strndup(arg, i);
		if (key == NULL)
            function_error(strndup);
		value = ft_substr(arg, i + 1, ft_strlen(arg) - i - 1);
		if (value == NULL)
			function_error(substr);
	}
    
}

int builtin_export(t_info *info)
{
    int i;
    if (info->argv[1] == NULL)
    {
        // 引数が与えられなかった場合には、現在の環境変数を表示
        char **env = info->envp;
        while (*env != NULL) {
            printf("%s\n", *env);
            env++;
        }
        return 0;
    }
    else
    {
        i = 1;
        while (info->argv[i])
		{
			export_env(info->argv[i]);
			i++;
		}
    }
    /*
    // 引数が与えられた場合には、環境変数を設定
    char *key = ft_split(args[1], "=");
    char *value = ft_split(NULL, "=");

    if (key == NULL || value == NULL) {
        fprintf(stderr, "Usage: export KEY=VALUE\n");
        return 1;
    }

    if (setenv(key, value, 1) != 0) {
        perror("export");
        return 1;
    }
    */
    return 0;
}