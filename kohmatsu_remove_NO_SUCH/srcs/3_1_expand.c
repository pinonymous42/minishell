/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_1_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 01:40:55 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/13 13:03:28 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	append_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = 2;
	if (*s)
		size += strlen(*s);
	new = malloc(sizeof(char) * size);
	if (new == NULL)
		function_error("malloc");
	if (*s)
		strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}

int	is_variable_character(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char    *double_variable_expand(char *new_word, t_environ *list)
{
    char    *tmp;
    char    *ret;
    char    *var;

	g_signal.do_split = 1;
    tmp = new_word;
	var = ft_calloc(sizeof(char), 1);
    while (*new_word != '$')
        new_word++;
    if (new_word == tmp)
        ret = ft_calloc(sizeof(char), 1);
    else
        ret = ft_strndup(tmp, new_word - tmp);
    new_word++;
    if (*new_word == '?')
    {
        ret = ft_strjoin_with_free(ret, ft_itoa(g_signal.status), FIRST_PARAM);
        new_word++;
    }
    else
    {
		if (ft_strchr(new_word, '$') || ft_strchr(new_word, ' ') || not_allowed_variant_character(new_word))
		{
			while (*new_word != '$' && *new_word != ' ' && is_variable_character(*new_word))
				append_char(&var, *new_word++);
		}
        else
            var = ft_strndup(new_word, ft_strchr(new_word, '\0') - new_word);
        ret = ft_strjoin_with_free(ret, search_env(var, list), FIRST_PARAM);
        free(var);
		if (ft_strchr(new_word, '$') || ft_strchr(new_word, ' '))
		{
			if (ft_strchr(new_word, ' ') == 0 || (ft_strchr(new_word, '$') && (ft_strchr(new_word, '$') < ft_strchr(new_word, ' '))))
				new_word = ft_strchr(new_word, '$');
			else if (ft_strchr(new_word, '$') == 0 || (ft_strchr(new_word, ' ') && (ft_strchr(new_word, '$') > ft_strchr(new_word, ' '))))
				new_word = ft_strchr(new_word, ' ');
		}
        else
		{
			if (not_allowed_variant_character(new_word))
			{
				while (is_variable_character(*new_word))
					new_word++;
			}
			else
            	new_word = ft_strchr(new_word, '\0');
		}
    }
    ret = ft_strjoin_with_free(ret, new_word, FIRST_PARAM);
    free(tmp);
    return (ret);
}

void	quote_removal(t_token *tok, t_environ *list)
{
	char	*new_word;
	char	*p;
	char	*var;

	if (tok == NULL || (tok->kind != TOKEN_WORD && tok->kind != TOKEN_METACHAR) || tok->word == NULL)
		return ;
	p = tok->word;
	new_word = NULL;
	while (*p)
	{
		if (*p == SINGLE_QUOTE)
		{
			p++;
			while (*p != SINGLE_QUOTE)
			{
				if (*p == '\0')
					assert_error("Unclosed single quote");
				if (*p == '$')
					g_signal.not_expand_flag = 1;
				append_char(&new_word, *p++);
			}
			p++;
		}
		else if (*p == DOUBLE_QUOTE)
		{
			p++;
			while (*p != DOUBLE_QUOTE)
			{
				if (*p == '\0')
					assert_error("Unclosed single quote");
				append_char(&new_word, *p++);
			}
			p++;
			while (ft_strchr(new_word, '$'))
				new_word = double_variable_expand(new_word, list);
		}
		else if (*p == '$')
        {
			g_signal.do_split = 1;
            while (ft_strchr(p, '$'))
            {
                p++;
                if (*p == '?')
                {
                    p++;
                    new_word = ft_itoa(g_signal.status);
                }
                else
                {
                    if (ft_strchr(p, '$'))
                        var = ft_strndup(p, ft_strchr(p, '$') - p - 1);
                    else
					{
						if (not_allowed_variant_character(p))
						{
							var = ft_calloc(sizeof(char), 1);
							while (is_variable_character(*p))
								append_char(&var, *p++);
						}
						else
							var = ft_strndup(p, ft_strchr(p, '\0') - p);
					}
                    if (new_word == NULL)
                        new_word = ft_strdup(search_env(var, list));
                    else
                        new_word = ft_strjoin_with_free(new_word, search_env(var, list), FIRST_PARAM);
                    free(var);
                    if (ft_strchr(p, '$'))
                        p = ft_strchr(p, '$');
                    else
					{
						if (not_allowed_variant_character(p))
						{
							while (is_variable_character(*p))
								p++;
						}
						else
                        	p = ft_strchr(p, '\0');
					}
                }
            }
        }
		else
		{
			if (ft_strncmp(tok->word, "<<", 2) == 0 && ft_strchr(tok->next->word, '\"') && ft_strncmp(tok->next->next->word, "<<", 2) == 1)
				g_signal.not_expand_flag = 1;
			append_char(&new_word, *p++);
		}
	}
	free(tok->word);
	tok->word = new_word;
	quote_removal(tok->next, list);
}

void	write_to_heredoc_one(char **array, int i, t_environ *list)
{
	char	*line;
	int		heredoc_fd;
	
	heredoc_fd = open(".heredoc", (O_WRONLY | O_CREAT | O_TRUNC), 0644);
    if (heredoc_fd == -1)
        function_error("open");
	heredoc_signal();
    while (1)
    {
        line = readline("> ");
		if (line == NULL)
			break ;
		if (ft_strncmp(line, array[i + 1], ft_strlen(line)) == 0)
		{
			free(line);
            break ;
		}
		if (g_signal.not_expand_flag == 0 && ft_strchr(line, '$'))
		{
			while (ft_strchr(line, '$'))
				line = double_variable_expand(line, list);
		}
        write(heredoc_fd, line, ft_strlen(line));
        write(heredoc_fd, "\n", 1);
		free(line);
    }
	dup2(g_signal.input_fd, 0);
	close(g_signal.input_fd);
    close(heredoc_fd);
}

void	write_to_heredoc_not_one(char **array, int i, int *heredoc_flag, t_environ *list)
{
	char	*line;
	int		heredoc_fd;
	char	*tmp;

	if (ft_strncmp(array[i + 4], "<<", 2) == 0)
        return ;
	heredoc_signal();
    *heredoc_flag = 1;
    heredoc_fd = open(".heredoc", (O_WRONLY | O_CREAT | O_TRUNC), 0644);
    if (heredoc_fd == -1)
        function_error("open");
    while (1)
    {
        line = readline("> ");
		if (line == NULL)
			break ;
        if (ft_strncmp(line, array[i + 1], ft_strlen(array[i + 1])) == 0)
		{
			free(line);
            break ;
		}
        free(line);
    }
	if (g_signal.other_code == FALSE)
	{
		while (1)
		{
			line = readline("> ");
			if (line == NULL)
				break ;
			if (ft_strncmp(line, array[i + 3], ft_strlen(array[i + 3])) == 0)
			{
				free(line);
				break ;
			}
			if (g_signal.not_expand_flag == 0 && search_env(line + 1, list))
			{
				tmp = search_env(line + 1, list);
				free(line);
				line = tmp;
			}
			write(heredoc_fd, line, ft_strlen(line));
			write(heredoc_fd, "\n", 1);
			free(line);
		}
	}
    dup2(g_signal.input_fd, 0);
	close(g_signal.input_fd);
    close(heredoc_fd);
}

char	**expand(t_token *tok, t_environ *list)
{
	char **array;
	int	heredoc_count;
	int	i;
	int	heredoc_flag;
	
	i = 0;
	quote_removal(tok, list);
	array = token_list_to_array(tok);
	heredoc_count = count_heredoc(array);
	heredoc_flag = 0;
	while (array[i])
	{
		 if (ft_strncmp(array[i], "<<", 2) == 0 && heredoc_flag == 0)
        {
            if (heredoc_count == 1)
                write_to_heredoc_one(array, i, list);
            else
                write_to_heredoc_not_one(array, i, &heredoc_flag, list);
        }
		i++;
	}
	return (array);
}