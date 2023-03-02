/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 22:43:55 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/28 12:49:30 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "../libgnl/get_next_line.h"
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
# include <stdarg.h>

# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '"'

# define RIGHT 1
# define MID 0
# define LEFT -1

# define STDIN 0
# define STDOUT 1

# define TRUE 1
# define FALSE 0

typedef enum e_error_kind{
    TOKENIZE_ERROR,
}t_error_kind;

typedef enum e_token_kind{
	TOKEN_WORD,
    TOKEN_METACHAR,
	TOKEN_CONTROL_OP,
	TOKEN_EOF,
}t_token_kind;

typedef struct s_token t_token;

struct s_token{
	char			*word;
	t_token_kind	kind;
	t_token			*next;
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
    int    heredoc_flag;
    // int     heredoc_count; 
}t_info;

typedef struct s_environ {
    char *key;
    char *value;
    struct s_environ *next;
}t_environ;

typedef struct s_signal {
    int status;//status code
    int heredoc_fd;//.heredocのfd
    int input_fd;//input(0)のfd
    int other_code;//status codeが0以外の時に用いる
}t_signal;

//global variable
t_signal g_signal;

//token_list_to_array.c
char	**token_list_to_array(t_token *token);

//destruter_ref.c
void	free_token(t_token *token);
void	free_array(char **array);

//error_ref.c
void    function_error(char *function_name);
void    tokenize_error(char *message, char **rest, char *line);
void	assert_error(const char *msg);
void	err_exit(const char *location, const char *msg);

//pipe_ref.c
void     pipex(int argc, char *argv[], t_environ *list);
int count_heredoc(char **argv);

//tokenize_ref.c
t_token *tokenize(char *line);
bool    is_metacharacter(char c);
char *ft_strndup(char *str, size_t n);

//expand_ref.c
char	**expand(t_token *tok, t_environ *list);

//make_environ.c
t_environ *make_environ(char **envp);

// signal.c
int set_signal(void);
int set_signal_child(void);
int set_signal_parent(void);
int    heredoc_signal(void);


// search_env.c
char *search_env(char *key, t_environ *list);

#endif