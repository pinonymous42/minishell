/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_ref.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:33:36 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/24 13:18:29 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_count_and_putstr(int fd, char *s)
{
	if (s == NULL)
		s = "(null)";
	write(fd, s, ft_strlen(s));
	return (ft_strlen(s));
}

int	ft_count_and_output(int fd, va_list ap, int fmt)
{
	int	add_count;

	add_count = 0;
	if (fmt == 's')
		add_count = ft_count_and_putstr(fd, (char *)va_arg(ap, char *));
	return (add_count);
}
int	count(int fd, va_list ap, const char *fmt)
{
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	while (fmt[i] != '\0')
	{
		if (fmt[i] != '%')
		{
			write(fd, &fmt[i], 1);
			count++;
		}
		else if (fmt[i] == '%')
		{
			i++;
			count += ft_count_and_output(fd, ap, fmt[i]);
		}
		i++;
	}
	return (count);
}

int my_dprintf(int fd, const char *fmt, ...)
{
    va_list	ap;
	int		length;

    va_start(ap, fmt);
	length = count(fd, ap, fmt);
    va_end(ap);
    return (length);
}

void    function_error(char *function_name)
{
    printf("function error: %s\n", function_name);
    exit(EXIT_FAILURE);
}

//exitしない
void    tokenize_error(char *message, char **rest, char *line, bool *syntax_error)
{
    *syntax_error = true;
    printf("minishell: syntax error near %s\n", message);
    while (*line)
		line++;
	*rest = line;
}

void	assert_error(const char *msg)
{
	my_dprintf(STDERR_FILENO, "Assert Error: %s\n", msg);
	exit(255);
}

void	err_exit(const char *location, const char *msg, int status)
{
	my_dprintf(STDERR_FILENO, "minishell: %s: %s\n", location, msg);
	exit(status);
}