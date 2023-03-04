/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 13:27:36 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/03 22:50:05 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *search_env(char *key, t_environ *list)
{
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
    // printf("%s, %d\n", __FILE__, __LINE__);
    return (NULL);
}