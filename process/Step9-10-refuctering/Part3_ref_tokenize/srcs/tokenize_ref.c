/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_ref.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:17:39 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/22 18:09:02 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token *create_new_token(char *word, t_token_kind kind)
{
    t_token *new_token;
    
    new_token = ft_calloc(1, sizeof(*new_token));
    if (new_token == NULL)
        function_error("ft_calloc");
    new_token->word = word;
    new_token->kind = kind;
    return(new_token);
}

bool is_blank(char c)
{
    if (c == ' ' || c == '\t' || c == '\n')
        return (true);
    else
        return (false);
}

void skip_blank(char **rest, char *line)
{
    while (*line != '\0' && is_blank(*line) == true)
        line++;
    *rest = line;
}

bool startswith(const char *s, const char *keyword)
{
    if (memcmp(s, keyword, strlen(keyword)) == 0)
        return (true);
    else
        return (false);
}

bool is_metacharacter(char c)
{
    if (c != 0 && strchr("|&;()<> \t\n", c) != NULL)
        return (true);
    else
        return (false);
}

t_token *metacharcter(char **rest, char *line)
{
    static char *const operators[] = {">>", "<<", "|", "&", ";", "(", ")", "<", ">","\n"};
    size_t  i = 0;
    char    *op;

    while (i < sizeof(operators) / sizeof(*operators))
    {
        if(startswith(line, operators[i]) == true)
        {
            op = strdup(operators[i]);
            if (op == NULL)
                function_error("strdup");
            *rest = line + strlen(op);
            return (create_new_token(op, TOKEN_METACHAR));
        }
        i++;
    }
    assert_error("Unexpected operator");
}

bool is_control_operator(const char *s)
{
    static char *const operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
    size_t  i = 0;

    while (i < sizeof(operators) / sizeof(*operators))
    {
        if (startswith(s, operators[i]))
            return (true);
        i++;
    }
    return (false);
}

t_token *control_operator(char **rest, char *line)
{
    static char *const operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
    size_t  i = 0;
    char    *op;

    while (i < sizeof(operators) / sizeof(*operators))
    {
        if(startswith(line, operators[i]) == true)
        {
            op = strdup(operators[i]);
            if (op == NULL)
                function_error("strdup");
            *rest = line + strlen(op);
            return (create_new_token(op, TOKEN_CONTROL_OP));
        }
        i++;
    }
    assert_error("Unexpected operator");
}

bool is_word(const char *s)
{
    if (*s != '\0' && is_metacharacter(*s) == false)
        return (true);
    else
        return (false);
}

t_token *word(char **rest, char *line)
{
    const char	*start = line;
	char		*word;

    while (*line && !is_metacharacter(*line))
	{
		if (*line == SINGLE_QUOTE)
		{
			// skip quote
			line++;
            while (*line && *line != SINGLE_QUOTE)
				line++;
            if (*line == '\0')
			{
				tokenize_error("Unclosed single quote", &line, line);
				break ;
            }
			//skip quote
            else
				line++;
		}
		else if (*line == DOUBLE_QUOTE)
		{
			// skip quote
			line++;
            while (*line && *line != DOUBLE_QUOTE)
				line++;
            if (*line == '\0')
			{
				tokenize_error("Unclosed double quote", &line, line);
				break ;
            }
            // skip quote
            else
				line++;
		}
		else
			line++;
    }
    word = strndup(start, line - start);
	if (word == NULL)
		function_error("strndup");
	*rest = line;
	return (create_new_token(word, TOKEN_WORD));
}

t_token *tokenize(char *line)
{
    t_token *head;
    t_token *token;
    t_token *new_token;
    
    head = ft_calloc(1, sizeof(*head)); // headの初期化
    if (head == NULL)
        function_error("ft_calloc");
    head->next = NULL;
    token = head;
    while (*line != '\0')
    {
        if (is_blank(*line) == true)
            skip_blank(&line, line);
        else if (is_metacharacter(*line) == true)
        {
            new_token = metacharcter(&line, line);
            token->next = new_token;
            token = token->next;
        }
        else if (is_control_operator(line) == true)
        {
            new_token = control_operator(&line, line);
            token->next = new_token;
            token = token->next;
        }
        else if (is_word(line) == true)
        {
            new_token = word(&line, line);
            token->next = new_token;
            token = token->next;
        }
        else
            tokenize_error("Unexpected Token", &line, line);
    }
    token->next = create_new_token(NULL, TOKEN_EOF);
    return (head->next);
}