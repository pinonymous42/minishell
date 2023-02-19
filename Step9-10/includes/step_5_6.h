/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   step_5_6.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 16:18:59 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/14 13:14:06 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STEP_5_6_H
# define STEP_5_6_H

# include <stdbool.h>
# include <stddef.h>
# include "step_1_4.h"

# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '"'

//error.c
# define ERROR_TOKENIZE 258

extern bool	syntax_error;

void	fatal_error(const char *msg) __attribute__((noreturn));
void	assert_error(const char *msg) __attribute__((noreturn));
void	err_exit(const char *location, const char *msg, int status) __attribute__((noreturn));
void	todo(const char *msg) __attribute__((noreturn));
void	tokenize_error(const char *location, char **rest, char *line);

//tokenize.c
typedef struct s_token		t_token;
enum e_token_kind {
	TK_WORD,//単語
	TK_RESERVED,//予約後
	TK_OP,//演算子
	TK_EOF,//終端
};
typedef enum e_token_kind	t_token_kind;

struct s_token {
	char			*word;
	t_token_kind	kind;
	t_token			*next;
};

t_token *new_token(char *word, t_token_kind kind);
bool    is_blank(char c);
bool    consume_blank(char **rest, char *line);
bool	startswith(const char *s, const char *keyword);
bool 	is_operator(const char *s);
bool 	is_metacharacter(char c);
bool 	is_word(const char *s);
t_token *operator(char **rest, char *line);
t_token *word(char **rest, char *line);

t_token *tokenize(char *line);

char	**tail_recursive(t_token *token, int nargs, char **argv);
char	**token_list_to_argv(t_token *token);

// destructor.c
void free_all_token(t_token *token);
void free_argv(char **argv);

// //expand.c
// void	append_char(char **s, char c);
// void    quote_removal(t_token *token);
// void    expand(t_node *node);

#endif