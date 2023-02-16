/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   step_9_10.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 11:44:49 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/16 20:02:19 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STEP_9_10_H
# define STEP_9_10_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <string.h>

# include <stdbool.h>
# include <stddef.h>

# include <fcntl.h>

# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '"'

//error.c
# define ERROR_TOKENIZE 258
# define ERROR_PARSE 258
# define ERROR_OPEN_REDIR 1

typedef struct s_token		t_token;
enum e_token_kind {
	TK_WORD,
	TK_RESERVED, 
	TK_OP,
	TK_EOF,
};

typedef enum e_token_kind	t_token_kind;

struct s_token {
	char			*word;
	t_token_kind	kind;
	t_token			*next;
};

enum e_node_kind{
    ND_SIMPLE_CMD,
	ND_REDIR_OUT,
	ND_REDIR_IN,
	ND_REDIR_APPEND,
};

typedef enum e_node_kind t_node_kind;

typedef struct s_node   t_node;
struct s_node{
    t_node_kind kind;
    t_node  *next;

	//CMD
	t_token *args;
	t_node	*redirects;

	//REDIR
	int		targetfd;
	t_token	*filename;
	int		filefd;
	int		stashed_targetfd;
};

// Redirecting output example
// command          : "echo hello 1 > out"
// targetfd         : 1
// filename         : "out"
// filefd           : open("out")
// stashed_targetfd : dup(targetfd)

// destructor.c
void    free_all_token(t_token *token);
void    free_argv(char **argv);
void    free_node(t_node *node);

//error.c
extern bool	syntax_error;

void	fatal_error(const char *msg) __attribute__((noreturn));
void	assert_error(const char *msg) __attribute__((noreturn));
void	err_exit(const char *location, const char *msg, int status) __attribute__((noreturn));
void	todo(const char *msg) __attribute__((noreturn));
void	tokenize_error(const char *location, char **rest, char *line);
void	parse_error(const char *location, t_token **rest, t_token *tok);
void	xperror(const char *location);

//expand.c
void	append_char(char **s, char c);
void    quote_removal(t_token *token);
void    expand(t_node *node);

// parse.c
bool	at_eof(t_token *token);
t_node	*new_node(t_node_kind kind);
t_token	*tokendup(t_token *token);
void	append_token(t_token **tokens, t_token *token);
void 	append_node(t_node **node, t_node *elm);
bool	equal_op(t_token *tok, char *op);
t_node	*redirect_out(t_token **rest, t_token *tok);
t_node	*redirect_in(t_token **rest, t_token *token);
t_node	*redirect_append(t_token **rest, t_token *token);
void	append_command_element(t_node *command, t_token **rest, t_token *token);
t_node	*parse(t_token *token);

//tokenize.c
t_token *new_token(char *word, t_token_kind kind);
bool    is_blank(char c);
bool    consume_blank(char **rest, char *line);
bool 	startswith(const char *s, const char *keyword);
bool 	is_operator(const char *s);
bool 	is_metacharacter(char c);
bool 	is_word(const char *s);
t_token *operator(char **rest, char *line);
t_token *word(char **rest, char *line);
t_token *tokenize(char *line);

char	**tail_recursive(t_token *token, int nargs, char **argv);
char	**token_list_to_argv(t_token *token);

//redirect.c
int		open_redir_file(t_node *redirects);
void	do_redirect(t_node *redirects);
void	reset_redirect(t_node *redirects);

#endif