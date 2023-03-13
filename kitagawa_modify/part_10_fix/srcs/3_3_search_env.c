/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_3_search_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 13:27:36 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/12 23:05:19 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_strcmp(char* str1, char* str2)
{
    while (*str1 == *str2)
    {
        if (*str1 == '\0' || *str2 == '\0')
            break;
        str1++;
        str2++;
    }
    
    if (*str1 == '\0' && *str2 == '\0')
        return 0;
    else
        return (*str1 - *str2);
}

char *search_env(char *key, t_environ *list)
{
    //char *no_such_env;
    //printf("%s, %d\n", __FILE__, __LINE__);
    //no_such_env = _ENV;
    while (list != NULL)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        if (ft_strcmp(list->key, key) == 0)
        {
            // printf("%s, %d\n", __FILE__, __LINE__);
            return (list->value);
        }
        list = list->next;
    }
    //printf("%s, %d\n", __FILE__, __LINE__);
    return (NO_SUCH_ENV);
}