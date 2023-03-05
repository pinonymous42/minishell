/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_6_export_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:34:51 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/05 18:06:08 by yokitaga         ###   ########.fr       */
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

void update_env(t_info *info, char *arg, t_environ *list)
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

void	add_new_env(t_info *info, char *arg, t_environ *list)
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

void    export_builtin(t_info *info, t_environ *list)
{
    int i;
    char    **env;
    char    *key;
    
    if (info->argv[1] == NULL)
    {
        //引数が与えられなかった場合には、現在の環境変数を表示
        env = list_to_array(info->list);
        
        while (*env != NULL) {
             ft_putendl_fd(*env, 1);
             env++;
        }
        return ;
    }
    i = 1;
    while (info->argv[i] != NULL)
    {
        key = ft_strndup(info->argv[1], ft_strchr_index(info->argv[1], '=')); // '='の前までをkeyとする
        if(search_env(key, list) != NULL)//既存の環境変数を上書きする場合
        {
            update_env(info, info->argv[i], list);
        }
        else //新しく追加する場合
            add_new_env(info, info->argv[i], list);
        free(key);
        i++;
    }
    /*
    else if(search_env(key, list) != NULL)
    {
        i = 1;
        while (info->argv[i] != NULL)
        {
            export_env(info, info->argv[i], list);
            i++;
        }
    }
    */
    //新しく追加する場合
    /*
    else
    {
        //info->updata_list = TRUE;
        //t_environ *new_list;
        //printf("%s, %d\n", __FILE__, __LINE__);
        //new_list = copy_list(info->list);
        //printf("%s, %d\n", __FILE__, __LINE__);
        i = 1;
        while (info->argv[i] != NULL)
		{
			export_env(info, info->argv[i], list);
			i++;
		}
        // while(info->list != NULL)
        // {
        //     printf("KEY:%s/", info->list->key);
        //     printf("VALUE:%s\n", info->list->value);
        //     info->list = info->list->next;
        // }
        //free_list(info->list);
        //info->list = new_list;
    }
    */
    return ;
}