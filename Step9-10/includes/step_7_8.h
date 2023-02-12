/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   step_7_8.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 17:47:37 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/10 23:58:31 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STEP_7_8_H
# define STEP_7_8_H

# include "step_5_6.h"

# define ERROR_PARSE 258

enum e_node_kind{
    ND_SIMPLE_CMD,
};
typedef enum e_node_kind t_node_kind;

typedef struct s_node   t_node;
struct s_node{
    t_token *args;
    t_node_kind kind;
    t_node  *next;
};

void	free_node(t_node *node);

//error.c
void	parse_error(const char *location, t_token **rest, t_token *tok);
void	xperror(const char *location);

// parse.c
t_node	*parse(t_token *tok);
bool	at_eof(t_token *tok);
t_node	*new_node(t_node_kind kind);
void	append_token(t_token **tokens, t_token *tok);
t_token	*tokendup(t_token *tok);

#endif