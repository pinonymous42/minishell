/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 17:46:55 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/14 16:06:35 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/step_7_8.h"

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
    // node->next = NULL;
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
    // if(!node->args)
    //     printf("nothing\n");
    // exit(1);
    while (token != NULL && at_eof(token) == false)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        if (token->kind == TK_WORD || token->kind == TK_OP)
        {
            append_token(&node->args, tokendup(token));
            token = token->next;
        }
        else
            parse_error("Unexpected Token", &token, token);
    }
    return(node);
}