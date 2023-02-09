/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 23:57:42 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/10 00:46:06 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/step_5_6.h"

void	append_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = 2;
	if (*s)
		size += strlen(*s);
	new = malloc(size);
	if (new == NULL)
		fatal_error("malloc");
	if (*s)
		strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}

void    quote_removal(t_token *token)
{
    char *new_word;
    char *p;
    
    if (token == NULL || token->kind != TK_WORD || token->word == NULL)
        return ;
    p = token->word;
    new_word = NULL;
    while (*p != '\0' && is_metacharacter(*p) == false)
    {
        if (*p == SINGLE_QUOTE)
        {
            //skip quote
            p++;
            while(*p != SINGLE_QUOTE)
            {
                if (*p == '\0')
                    todo("Unclosed single quote");
				append_char(&new_word, *p++);   
            }
            //skip quote
            p++;
        }
        else
          append_char(&new_word, *p++);  
    }
    free(token->word);
    token->word = new_word;
    quote_removal(token->next);
}

void    expand(t_token *token)
{
    quote_removal(token);
}