/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   step_1_4.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 22:30:37 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/25 22:17:11 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STEP_1_4_H
# define STEP_1_4_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <string.h>
# include <stdbool.h>
# include <stddef.h>
# include "pipex.h"
# include <signal.h>

# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '"'
//error.c
# define ERROR_TOKENIZE 258
# define ERROR_PARSE 258
#define RIGHT 1
#define MID 0
#define LEFT -1
extern bool	syntax_error;

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

typedef enum e_node_kind t_node_kind;
enum e_node_kind{
    ND_SIMPLE_CMD,
};

typedef struct s_node   t_node;
struct s_node{
    t_token *args;
    t_node_kind kind;
    t_node  *next;
};

typedef struct s_environ {
    char *key;
    char *value;
    struct s_environ *next;
}t_environ;

// //main.c
// int    loop(t_environ *list);

//tokenize.c
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

void	free_node(t_node *node);
//error.c
void	parse_error(const char *location, t_token **rest, t_token *tok);
void	xperror(const char *location);
void fatal_error(const char *msg);
void	assert_error(const char *msg);
void	err_exit(const char *location, const char *msg, int status);
void tokenize_error(const char *location, char **rest, char *line);

// parse.c
t_node	*parse(t_token *tok);
bool	at_eof(t_token *tok);
t_node	*new_node(t_node_kind kind);
void	append_token(t_token **tokens, t_token *tok);
t_token	*tokendup(t_token *tok);

//expand.c
void	append_char(char **s, char c);
void    quote_removal(t_token *token, int stat_loc, t_environ *list);
void    expand(t_node *node, int stat_loc, t_environ *list);

//pipex.c
int     pipex(int argc, char *argv[], t_environ *list);

//make_environ.c
t_environ *make_environ(char **envp);

// search_env.c
char *search_env(char *key, t_environ *list);

// signal.c
int set_signal(void);
int g_signal;


#endif