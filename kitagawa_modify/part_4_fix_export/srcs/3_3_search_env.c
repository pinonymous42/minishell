/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_3_search_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 13:27:36 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/08 23:02:43 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *search_env(char *key, t_environ *list)
{
    //char *no_such_env;
    //printf("%s, %d\n", __FILE__, __LINE__);
    //no_such_env = NO_SUCH_ENV;
    while (list != NULL)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        if (ft_strncmp(list->key, key, ft_strlen(key)) == 0)
        {
            // printf("%s, %d\n", __FILE__, __LINE__);
            return (list->value);
        }
        list = list->next;
    }
    //printf("%s, %d\n", __FILE__, __LINE__);
    return (NO_SUCH_ENV);
}