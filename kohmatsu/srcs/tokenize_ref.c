/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_ref.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 15:03:28 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/27 13:44:29 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool is_blank(char c)
{
    if (c == ' ' || c == '\t' || c == '\n')
        return (true);
    else
        return (false);
}

bool is_metacharacter(char c)
{
    if (c != 0 && ft_strchr("|&;()<> \t\n", c) != NULL)
        return (true);
    else
        return (false);
}

static bool is_control_operator(const char *s)
{
    static char *control_operators[9] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
    size_t  i;

    i = 0;
    while (i < 9)
    {
        if (ft_strncmp(s, control_operators[i], ft_strlen(control_operators[i])) == 0)
            return (true);
        i++;
    }
    return (false);
}

static t_token *new_token(t_token_kind kind)
{
    t_token *new_token;

    new_token = (t_token *)malloc(sizeof(t_token));
    if (new_token == NULL)
        function_error("malloc");
    new_token->kind = kind;
    new_token->word = NULL;
    new_token->next = NULL;
    return (new_token);
}

static void append_token(t_token **head, t_token *new)
{
    t_token *current;

    if (*head == NULL)
        *head = new;
    else
    {
        current = *head;
        while (current->next != NULL)
            current = current->next;
        current->next = new;
    }
}

static t_token *handle_metacharacter(char **rest, char *line)
{
    t_token *token;
    static char *metacharacters[9] = {">>", "<<", "|", "&", ";", "(", ")", "<", ">"};
    size_t  i;
    char    *metachar;

    i = 0;
    token = new_token(TOKEN_METACHAR);
    while (i < 9)
    {
        if (ft_strncmp(line, metacharacters[i], ft_strlen(metacharacters[i])) == 0)
        {
            metachar = ft_strdup(metacharacters[i]);
            if (metachar == NULL)
                function_error("strdup");
            *rest = line + ft_strlen(metachar);
            token->word = metachar;
            return (token);
        }
        i++;
    }
    assert_error("Unexpected operator");
}

static t_token *handle_contorol_op(char **rest, char *line)
{
    t_token *token;
    static char *control_operators[9] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
    size_t  i;
    char    *op;

    i = 0;
    token = new_token(TOKEN_CONTROL_OP);
    while (i < 9)
    {
        if (ft_strncmp(line, control_operators[i], ft_strlen(control_operators[i])) == 0)
        {
            op = ft_strdup(control_operators[i]);
            if (op == NULL)
                function_error("strdup");
            *rest = line + ft_strlen(op);
            token->word = op;
            return (token);
        }
        i++;
    }
    assert_error("Unexpected operator");
}

char *ft_strndup(char *str, size_t n)
{
    char    *new_str;
    size_t  len;

    if (str == NULL)
        return (NULL);
    len = ft_strlen(str);
    if (len > n)
        len = n;
    new_str = (char *)malloc(sizeof(char) * (len + 1));
    if (new_str == NULL)
        function_error("malloc");
    ft_memcpy(new_str, str, len);
    new_str[len] = '\0';

    return (new_str);
}

static t_token *handle_word(char **rest, char *line)
{
    t_token *token;
    char    *start;
    char    *end;

    token = new_token(TOKEN_WORD);
    start = line;
    while (*line != '\0' && is_metacharacter(*line) == false && is_control_operator(line) == false)
    {
        if (*line == SINGLE_QUOTE)
        {
            line++;
            while (*line && *line != SINGLE_QUOTE)
				line++;
            if (*line == '\0')
			{
				tokenize_error("Unclosed quote", &line, line);
				break ;
            }
            else
                line++;
        }
        else if (*line == DOUBLE_QUOTE)
        {
            line++;
            while (*line && *line != DOUBLE_QUOTE)
				line++;
            if (*line == '\0')
			{
				tokenize_error("Unclosed quote", &line, line);
				break ;
            }
            else
                line++;
        }
        else
            line++;
    }
    end = line;
    token->word = ft_strndup(start, end - start);
    if (token->word == NULL)
		function_error("strndup");
    *rest = line;
	return (token);
}

t_token *tokenize(char *line)
{
    t_token *head;
    t_token *token;

    head = NULL;
    while (*line != '\0')
    {
        if (is_blank(*line) == true)
            line++;
        else if (is_metacharacter(*line) == true)
        {
            token = handle_metacharacter(&line, line);
            append_token(&head, token);
        }
        else if (is_control_operator(line) == true)
        {
            token = handle_contorol_op(&line, line);
            append_token(&head, token);
        }
        else
        {
            token = handle_word(&line, line);
            append_token(&head, token);
        }
    }
    append_token(&head, new_token(TOKEN_EOF));
    return (head);
}