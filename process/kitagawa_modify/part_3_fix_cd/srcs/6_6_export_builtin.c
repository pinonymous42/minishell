/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_6_export_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:34:51 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/08 21:55:04 by yokitaga         ###   ########.fr       */
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


bool    check_argv_no_such_env(t_info *info)
{
    int i;

    i = 1;
    //全部の引数が展開されている場合はtrue
    //printf("%s, %d\n", __FILE__, __LINE__);
    //printf("%s\n", info->argv[1]);
    while (info->argv[i] != NULL)
    {
        if (ft_strncmp(info->argv[i], NO_SUCH_ENV, ft_strlen(NO_SUCH_ENV)) != 0)
            return (false);
        i++;
    }
    //printf("%s, %d\n", __FILE__, __LINE__);
    return (true);//全部の引数がNO_SUCH_ENVの場合はtrue
}


void    export_builtin(t_info *info, t_environ *list)
{
    int i;
    //char    **env;
    char    *key;
    //printf("%s, %d\n", __FILE__, __LINE__);
    if (info->argv[1] == NULL)
    {
        //引数が与えられなかった場合には、現在の環境変数を表示
        /*
        char    **env;
        int     i;
        
        env = list_to_array(list);
        i = 0;
        while (env[i] != NULL) {
            ft_putendl_fd(env[i], 1);
            i++;
        }
        free_array(env);
        */
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
    //printf("%d\n", check_argv_no_such_env(info));
    
    else if (check_argv_no_such_env(info) == true)
    {
        //printf("%s, %d\n", __FILE__, __LINE__);
        // 全て引数が展開されているかつ、全ての引数が既存の環境変数に含まれていない場合は引数なしと同じ
        // char **argv;
        // int   j;

        // argv = info->argv;
        // j = 1;
        // while (argv[j] != NULL)
        // {
        //     if (search_env(argv[j], list) != NULL) ///既存の変数と一致した場合
        //         break;
        //     j++;
        // }
        //ここに来た時点で、全ての引数が既存の環境変数にふくまれていないので、引数なしと同じ
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
    }
    else
    {
    //printf("%s, %d\n", __FILE__, __LINE__);
        i = 1;
        while (info->argv[i] != NULL)
        {
            key = ft_strndup(info->argv[1], ft_strchr_index(info->argv[1], '=')); // '='の前までをkeyとする
            if(search_env(key, list) != NO_SUCH_ENV)//既存の環境変数を上書きする場合
            {
                //printf("%s, %d\n", __FILE__, __LINE__);
                update_env(info, info->argv[i], list);
            }
            else //新しく追加する場合
            {
                //printf("%s, %d\n", __FILE__, __LINE__);
                add_new_env(info, info->argv[i], list);
            }
            free(key);
            i++;
        }
    }
    return ;
}