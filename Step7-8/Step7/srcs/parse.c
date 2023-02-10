/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 17:46:55 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/11 00:19:15 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/step_7_8.h"

bool	at_eof(t_token *tok)
{
	return (tok->kind == TK_EOF);
}

t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = calloc(1, sizeof(*node));
	if (node == NULL)
		fatal_error("calloc");
	node->kind = kind;
	return (node);
}

t_token *tokendup(t_token *token)
{
    char *word;
    
    word = strdup(token->word);
    if (word == NULL)
        fatal_error("strdup");
    return (new_token(word, token->kind));
}

void append_token(t_token **tokens, t_token *token)
{
    if (*tokens == NULL)
    {
        *tokens = token;
        return ;
    }
    append_token(&(*tokens)->next, token);
}

t_node *parse(t_token *token)
{
    t_node *node;

    node = new_node(ND_SIMPLE_CMD);
    while (token != NULL && at_eof(token) == false)
    {
        if (token->kind == TK_WORD)
        {
            append_token(&node->args, tokendup(token));
            token = token->next;
        }
        else
            parse_error("Unexpected Token", &token, token);
    }
    return(node);
}