/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_6_export_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:34:51 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/13 13:06:07 by kohmatsu         ###   ########.fr       */
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

t_environ *new_node(char *key, char *value)
{
    t_environ *new;
    
    //printf("%s, %d\n", __FILE__, __LINE__);
    new = malloc(sizeof(t_environ));
    if (!new)
        function_error("malloc");
    new->key = ft_strdup(key);
    // if (new->key == NULL)
    //     function_error("strdup");
    new->value = ft_strdup(value);
    // if (new->value == NULL)
    //     function_error("strdup");
    new->next = NULL;
    return (new);
}

void    list_add_back_export(t_environ **list, char *key, char *value)
{
    t_environ *head;
    
    head = *list;
    //printf("%s, %d\n", __FILE__, __LINE__);
    while ((*list)->next != NULL)
    {
        // printf("key:%s/", (*list)->key);
        // printf("value:%s\n", (*list)->value);
        *list = (*list)->next;
    }
    (*list)->next = new_node(key, value);
    // printf("new key:%s/", (*list)->next->key);
    // printf("new value:%s\n", (*list)->next->value);
    *list = head;
}

void update_env(char *arg, t_environ *list)
{
    char    *key;
    char    *value;
    int     i;

    i = ft_strchr_index(arg, '='); // '='のindexを取得
    if (i == -1)// '='がない場合は何もしない
        return ;
    else //'='がある場合
    {
        key = ft_strndup(arg, i);
        if(key == NULL)
            function_error("strndup");
        value = ft_substr(arg, i + 1, ft_strlen(arg) - i - 1);// '='の後ろの文字列をvalueとする
        while (list != NULL)
        {
            if (ft_strncmp(list->key, key, ft_strlen(key)) == 0)
            {
                free(list->value);
                list->value = value;
                break ;
            }
            list = list->next;
        }
    }
    free(key);
}

void	add_new_env(char *arg, t_environ *list)
{
	char	*key;
	char	*value;
	int		i;

	i = ft_strchr_index(arg, '=');
	if (i == -1)// '='がない場合
	{
        key = ft_strdup(arg);
        if (key == NULL)
            function_error("strdup");
        value = NULL;
        list_add_back_export(&list, key, value);
        free(key);
        //list_add_back_export(&info, key, value);
    }
    else
    {
        //iは'='のindex
        //printf("%s, %d\n", __FILE__, __LINE__);
        key = ft_strndup(arg, i);
        if (key == NULL)
            function_error("strndup");
        value = ft_substr(arg, i + 1, ft_strlen(arg) - i - 1); // '='の後ろの文字列をvalueとする
        if (value == NULL)
            function_error("substr");
        //printf("%s\n", key);
        //printf("%s\n", value);
        //list_add_back_export(&(info->list), key, value);
        list_add_back_export(&list, key, value);
        free(key);
        free(value);
        /*
        while (info->list != NULL)
        {
            printf("KEY:%s/", info->list->key);
            printf("VALUE:%s\n", info->list->value);
            info->list = info->list->next;
        }
        */
    //     while(list != NULL)
    //     {
    //         printf("KEY:%s/", list->key);
    //         printf("VALUE:%s\n", list->value);
    //         list = list->next;
    //     }
    }
}

//修正必要
bool    check_argv_no_such_env(t_info *info)
{
    int i;

    i = 1;
    //全部の引数が展開されている場合はtrue
    //printf("%s, %d\n", __FILE__, __LINE__);
    //printf("%s\n", info->argv[1]);
    while (info->argv[i] != NULL)
    {
        if (ft_strchr(info->argv[i], '$') == NULL)//引数に'$'がない場合
            return (false);
        i++;
    }
    //printf("%s, %d\n", __FILE__, __LINE__);
    return (true);//全部の引数に'$'がある場合
}

bool check_add_or_not(char *arg, t_environ *list)
{
    int i;

    i = 0;
    char *key;
    while (arg[i] != '\0')
    {
        if (arg[i] == '+')
        {
            if (arg[i + 1] == '=')
            {
                key = ft_strndup(arg, i);// '+'の前の文字列をkeyとする
                if(search_env(key, list) != NULL)//keyが存在する場合
                {
                    free(key);
                    return (true);
                }
                else
                {
                    free(key);
                    return (false);
                }
            }
        }
        i++;
    }
    return (false);
}

void    add_env_value(char *arg, t_environ *list)
{
    int  i;
    char *key;

    i = ft_strchr_index(arg, '+');
    key = ft_strndup(arg, i);
    while (list != NULL)
    {
        if (ft_strncmp(list->key, key, ft_strlen(key)) == 0)
        {
            list->value = ft_strjoin_with_free(list->value, arg + i + 2, 1);
            break ;
        }
        list = list->next;
    }
    free(key);
}

int    not_allowed_variant_character(char *key)
{
    while (*key)
    {
        if (!(ft_isalnum(*key) || *key == '_'))
            return (1);
        key++;
    }
    return (0);
}

void    export_builtin(t_info *info, t_environ *list)
{
    int i;
    //char    **env;
    char    *key;
    //printf("%s, %d\n", __FILE__, __LINE__);
    if (info->argv[1] == NULL || check_argv_no_such_env(info) == true)
    {
        t_environ *tmp;
        tmp = list;
        while (tmp != NULL)
        {
            ft_putstr_fd("declare -x ", 1);
            ft_putstr_fd(tmp->key, 1);
            if (tmp->value != NULL)
            {
                ft_putstr_fd("=\"", 1);
                ft_putstr_fd(tmp->value, 1);
                ft_putstr_fd("\"", 1);
            }
            ft_putstr_fd("\n", 1);
            tmp = tmp->next;
        }
        return ;
    }
    else
    {
        i = 1;
        while (info->argv[i] != NULL)
        {
            if (check_add_or_not(info->argv[i], list) == true)
            {
                //printf("%s, %d\n", __FILE__, __LINE__);
                add_env_value(info->argv[i], list);
                i++;
            }
            else
            {
                //printf("%d\n", ft_strchr_index(info->argv[1], '='));
                if (ft_strchr_index(info->argv[1], '=') != -1)// '='がある場合
                    key = ft_strndup(info->argv[1], ft_strchr_index(info->argv[1], '=')); // '='の前までをkeyとする
                else
                    key = ft_strdup(info->argv[1]);// '='がない場合はそのままkeyとする
                if (not_allowed_variant_character(key))
                {
                    my_dprintf(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", info->argv[1]);
                    g_signal.status = 1;
                    g_signal.other_code = TRUE;
                    return ;
                }
                if(search_env(key, list) != NULL)//keyが存在する場合
                    update_env(info->argv[i], list);
                else //新しく追加する場合
                    add_new_env(info->argv[i], list);
                free(key);
                i++;
            }
        }
    }
    return ;
}