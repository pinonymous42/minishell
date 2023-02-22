/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   step_9_10.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 10:52:47 by matsushimak       #+#    #+#             */
/*   Updated: 2023/02/22 00:56:28 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STEP_9_10_H
# define STEP_9_10_H

#include "../libft/libft.h"
#include "../libgnl/get_next_line.h"

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

#define RIGHT 1
#define MID 0
#define LEFT -1

#define STDIN 0
#define STDOUT 1

# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '"'

# define ERROR_TOKENIZE 258
# define ERROR_PARSE 258
# define ERROR_OPEN_REDIR 1

/*
typedef enum e_token_kind{
	WORD,
	//RESERED,
    METACHAR,
	CONTROL_OP,
	TK_EOF,
}t_token_kind;
*/

typedef enum e_token_kind{
	TK_WORD,
	TK_RESERVED, 
	TK_OP,
	TK_EOF,
}t_token_kind;

typedef struct s_token t_token;

struct s_token{
	char			*word;
	t_token_kind	kind;
	t_token			*next;
};

typedef enum e_node_kind{
    ND_SIMPLE_CMD,
}t_node_kind;

typedef struct s_node t_node;

struct s_node{
    t_node_kind kind;
    t_node  *next;

	//CMD
	t_token *args;
	t_node	*redirects;

	//REDIR
	int		targetfd;
	t_token	*filename;
	t_token *delimiter;
	int		filefd;
	int		stashed_targetfd;
};

typedef struct s_info{
    int     input_fd;
    int     output_fd;
    int     argc;//cmdの数
    int     pipe_count;//(pipeの個数)
    char    **cmd;//実行コマンド(全部)
    char    **argv;//実行コマンド(部分的)
    int     argv_count;//実行コマンドの数(部分的)
    char    **path;//search path
    char    **envp;//環境変数
    int     *pipe_place;//pipeの位置インデックス
    // char    *heredoc_str;
}t_info;

// destructor.c
void    free_all_token(t_token *token);
void    free_argv(char **argv);
//void    free_node(t_node *node);

//error.c
extern bool	syntax_error;

void	fatal_error(const char *msg) __attribute__((noreturn));
void	assert_error(const char *msg) __attribute__((noreturn));
void	err_exit(const char *location, const char *msg, int status) __attribute__((noreturn));
void	todo(const char *msg) __attribute__((noreturn));
void	tokenize_error(const char *location, char **rest, char *line);
//void	parse_error(const char *location, t_token **rest, t_token *tok);
void	xperror(const char *location);

//expand.c
void	append_char(char **s, char c);
void    quote_removal(t_token *token);
//void	expand_quote_removal(t_node *node);
//void  expand(t_node *node);
void	expand_quote_removal(t_token *token);
void    expand(t_token *token);

//parse.c
bool	at_eof(t_token *tok);
t_node	*new_node(t_node_kind kind);
t_token *tokendup(t_token *token);
void append_token(t_token **tokens, t_token *token);
t_node *parse(t_token *token);

//tokenize.c
t_token *new_token(char *word, t_token_kind kind);
bool    is_blank(char c);
bool    consume_blank(char **rest, char *line);
bool 	startswith(const char *s, const char *keyword);
//bool 	is_operator(const char *s);
bool 	is_metacharacter(char c);
bool 	is_control_operator(const char *s);
bool 	is_word(const char *s);
t_token *operator(char **rest, char *line);
t_token *control_operator(char **rest, char *line);
t_token *word(char **rest, char *line);
t_token *tokenize(char *line);
char	**tail_recursive(t_token *token, int nargs, char **argv);
char	**token_list_to_argv(t_token *token);

int     pipex(int argc, char *argv[]);
// void    safty_free(char **str);
#endif
