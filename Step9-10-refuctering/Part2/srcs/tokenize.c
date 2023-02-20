/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 17:33:58 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/20 16:50:49 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/step_9_10.h"

t_token *new_token(char *word, t_token_kind kind)
{
    t_token *token;

    token = calloc (1, sizeof(*token));
    if (token == NULL)
        fatal_error("calloc");
    token->word = word;
    token->kind = kind;
    // token->next = NULL;
    return (token);
}

bool    is_blank(char c)
{
    if (c == ' ' || c == '\t' || c == '\n')
        return (true);
    else
        return (false);
}

bool    consume_blank(char **rest, char *line)
{
    if (is_blank(*line) == true)
    {
        while (*line != '\0' && is_blank(*line) == true)
            line++;
        *rest = line;
        return (true);
    }
    *rest = line;
    return (false);
}

bool startswith(const char *s, const char *keyword)
{
    if (memcmp(s, keyword, strlen(keyword)) == 0)
        return (true);
    else
        return (false);
}

bool is_operator(const char *s)
{
    static char *const operators[] = {"|", "||", "&", "&&", ";", ";;", "(", ")", "<", "<<", ">", ">>", "\n"};
    size_t  i = 0;

    while (i < sizeof(operators) / sizeof(*operators))
    {
        if (startswith(s, operators[i]))
            return (true);
        i++;
    }
    return (false);
}

bool is_metacharacter(char c)
{
    if (c != 0 && strchr("|&;()<> \t\n", c) != NULL)
        return (true);
    else
        return (false);
}

bool is_word(const char *s)
{
    if (*s != '\0' && is_metacharacter(*s) == false)
        return (true);
    else
        return (false);
}

t_token *operator(char **rest, char *line)
{
    static char *const operators[] = {"|", "||", "&", "&&", ";", ";;", "(", ")", "<", "<<", ">", ">>", "\n"};
    size_t  i = 0;
    char    *op;

    while (i < sizeof(operators) / sizeof(*operators))
    {
        if(startswith(line, operators[i]) == true)
        {
            if (startswith(line, operators[i + 1]) == true)
                i++;
            op = strdup(operators[i]);
            if (op == NULL)
                fatal_error("strdup");
            *rest = line + strlen(op);
            return (new_token(op, TK_OP));
        }
        i++;
    }
    assert_error("Unexpected operator");
}

t_token *word(char **rest, char *line)
{
    const char  *start = line;
    char        *word;

    while (*line != '\0' && is_metacharacter(*line) == false)
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
			// skip quote
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
        fatal_error("strndup");
    *rest = line;
    return (new_token(word, TK_WORD));
}

t_token *tokenize(char *line)
{
    t_token head;
    t_token *token;
    t_token *new;

    syntax_error = false;
    head.next = NULL;
    token = &head;
    while(*line != '\0')
    {
        if (consume_blank(&line, line) == true)
            continue;
        else if (is_operator(line) == true)
        { 
            new = operator(&line, line);
            token->next = new;
            token = token->next;
        }
        else if (is_word(line) == true)
        {
            new = word(&line, line);
            token->next = new;
            token = token->next;
        }
        else
            tokenize_error("Unexpected Token", &line, line);
        // printf("%s, %d\n", __FILE__, __LINE__);
    }
    token->next = new_token(NULL, TK_EOF);
    // printf("%s\n", token->next->next);
    // if (!token->next->next)
    //     printf("nothing!\n");
    // exit(1);
    return (head.next);
}

char	**tail_recursive(t_token *token, int nargs, char **argv)
{
	if (token == NULL || token->kind == TK_EOF)
		return (argv);
	argv = reallocf(argv, (nargs + 2) * sizeof(char *));
	argv[nargs] = strdup(token->word);
	if (argv[nargs] == NULL)
		fatal_error("strdup");
	argv[nargs + 1] = NULL;
	return (tail_recursive(token->next, nargs + 1, argv));
}

char	**token_list_to_argv(t_token *token)
{
	char	**argv;

	argv = calloc(1, sizeof(char *));
	if (argv == NULL)
		fatal_error("calloc");
	return (tail_recursive(token, 0, argv));
}
