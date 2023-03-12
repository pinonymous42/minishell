/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_3_search_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 13:27:36 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/11 15:01:27 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *search_env(char *key, t_environ *list)
{
    //char *no_such_env;
    //printf("%s, %d\n", __FILE__, __LINE__);
    //no_such_env = _ENV;
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