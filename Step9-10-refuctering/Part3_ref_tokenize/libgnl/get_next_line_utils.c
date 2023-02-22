/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matsushimakouhei <matsushimakouhei@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 18:00:56 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/02/12 12:20:27 by matsushimak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	d_len;
	size_t	s_len;
	size_t	i;

	i = 0;
	s_len = ft_strlen(src);
	if (dst == NULL && dstsize == 0)
		return (s_len);
	d_len = ft_strlen(dst);
	if (d_len >= dstsize)
		return (s_len + dstsize);
	while (*dst)
		dst++;
	while (src[i] && i < (dstsize - d_len - 1))
	{
		*dst = src[i];
		dst++;
		i++;
	}
	*dst = '\0';
	return (s_len + d_len);
}

void	get_it_freed(char **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

char	*ft_strjoin_with_free(char *s1, char *s2, int n)
{
	char	*ret;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	ret = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!ret)
		return (NULL);
	*ret = '\0';
	ft_strlcat(ret, s1, ft_strlen(s1) + 1);
	ft_strlcat(ret, s2, ft_strlen(s1) + ft_strlen(s2) + 1);
	if (n == 1)
		get_it_freed(&s1);
	if (n == 2)
	{
		get_it_freed(&s1);
		get_it_freed(&s2);
	}
	return (ret);
}
