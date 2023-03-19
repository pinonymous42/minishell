/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_3_search_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 13:27:36 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/03/13 13:02:35 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *search_env(char *key, t_environ *list)
{
    while (list != NULL)
    {
        if (ft_strncmp(list->key, key, ft_strlen(key)) == 0)
            return (list->value);
        list = list->next;
    }
    return (NULL);
}