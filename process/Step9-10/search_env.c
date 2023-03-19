/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 19:13:52 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/02/22 19:39:18 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/step_1_4.h"

char *search_env(char *key, t_environ *list)
{
    while (list != NULL)
    {
        if (ft_strncmp(list->key, key, ft_strlen(key)) == 0)
        {
            // printf("%s, %d\n", __FILE__, __LINE__);
            return (list->value);
        }
        list = list->next;
    }
    // printf("%s, %d\n", __FILE__, __LINE__);
    return ("");
}