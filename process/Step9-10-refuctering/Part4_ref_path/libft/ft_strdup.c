/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 14:33:16 by matsushimak       #+#    #+#             */
/*   Updated: 2022/10/18 21:24:42 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*ret;
	char	*lead;

	ret = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!ret)
		return (NULL);
	lead = ret;
	while (*s1)
	{
		*ret = *s1;
		ret++;
		s1++;
	}
	*ret = '\0';
	return (lead);
}

// #include <stdio.h>
// #include <stdlib.h>

// int main(void)
// {
// 	char *s1 = "abc";
// 	printf("%p\n", s1);
// 	printf("%p\n", ft_strdup(s1));
// 	return (0);
// }