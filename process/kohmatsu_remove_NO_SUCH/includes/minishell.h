/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 22:43:55 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/13 12:15:54 by kohmatsu         ###   ########.fr       */
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
// # define NO_SUCH_ENV  "No_such_environment_variable"

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

typedef struct s_environ {
    char *key;
    char *value;
    struct s_environ *next;
}t_environ;

typedef struct s_info{
    int     input_fd;
    int     output_fd;
    int     **pipefd;
    int     argc;//cmdの数
    char    **cmd;//実行コマンド(全部)
    char    **argv;//実行コマンド(部分的)
    int     argv_count;//実行コマンドの数(部分的)
    char    **path;//search path
    // char    **envp;//環境変数
    //bool    updata_list;//環境変数の更新があったかどうか
    t_environ *list;
    int    heredoc_flag;//heredocを行なったかどうか
    int *pipe_place;//pipeの位置インデックス
    // int     heredoc_count; 
}t_info;


typedef struct s_signal {
    int status;//status code
    int heredoc_fd;//.heredocのfd
    int input_fd;//input(0)のfd
    int output_fd;//output(1)のfd
    int other_code;//status codeが0以外の時に用いる
    int do_split;//splitを行うかどうか
    int pipe_count;//(pipeの個数)
    int not_expand_flag;
}t_signal;

//global variable
t_signal g_signal;

//token_list_to_array.c
char	**token_list_to_array(t_token *token);

//destruter_ref.c
void	free_token(t_token *token);
void	free_array(char **array);
void	free_list(t_environ *list);

//error_ref.c
void    function_error(char *function_name);
void    tokenize_error(char *message, char **rest, char *line);
void    tokenize_error_2(char *message, char *word);
void	assert_error(const char *msg);
void	err_exit(const char *location, const char *msg);
void	command_not_found(const char *location);
void	file_not_found(const char *filename);
int     my_dprintf(int fd, const char *fmt, ...);
void    ambiguous_redirect(char *filename);

//pipe.c
void     pipex(int argc, char *argv[], t_environ *list);
int count_heredoc(char **argv);
char **list_to_array(t_environ *list);
int count_pipe(t_token *token);

//tokenize_ref.c
t_token *tokenize(char *line);
bool    is_metacharacter(char c);
char *ft_strndup(char *str, size_t n);

//expand_ref.c
char	**expand(t_token *tok, t_environ *list);

//make_environ.c
t_environ *make_environ(char **envp);
t_environ *new_list(char *envp);
void      list_add_back(t_environ **list, t_environ *new);

// signal.c
int set_signal(void);
int set_signal_child(void);
int set_signal_parent(void);
int heredoc_signal(void);


// search_env.c
char *search_env(char *key, t_environ *list);

//cd_builtin.c
void    cd_builtin(t_info *info);

//echo_builtin.c
void    echo_builtin(t_info *info, int j);

//env_builtin.c
void    env_builtin(t_info *info);

// exit_builtin.c
void    exit_builtin(t_info *info);

// pwd_builtin.c
void    pwd_builtin(t_info *info);

//export_builtin.c
int	    ft_strchr_index(const char *str, char c);
void    export_builtin(t_info *info, t_environ *list);
int     not_allowed_variant_character(char *key);

//unset_builtin.c
void    unset_builtin(t_info *info, t_environ *list);

#endif