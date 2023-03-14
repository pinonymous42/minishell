/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 22:43:55 by yokitaga          #+#    #+#             */
/*   Updated: 2023/03/14 21:10:10 by yokitaga         ###   ########.fr       */
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
void    export_not_valid_identifier(char *argv);
void	err_exit(const char *location, const char *msg);
void	command_not_found(const char *location);
void	file_not_found(const char *filename);
void    ambiguous_redirect(char *filename);
int     my_dprintf(int fd, const char *fmt, ...);

//pipe.c
void     pipex(int argc, char *argv[], t_environ *list);
int count_heredoc(char **argv);
char **list_to_array(t_environ *list);
int count_pipe(t_token *token);

//tokenize
//2_1_tokenize.c
t_token *tokenize(char *line);
//2_2_tokenize_utils.c
bool	is_blank(char c);
bool    is_metacharacter(char c);
bool	is_control_operator(const char *s);
t_token	*new_token(t_token_kind kind);
void	append_token(t_token **head, t_token *new);
//2_3_tokenize_handle.c
t_token	*handle_metacharacter(char **rest, char *line);
t_token	*handle_contorol_op(char **rest, char *line);
//2_4_tokenize_handle_word.c
char    *ft_strndup(char *str, size_t n);
bool	check_single_quote(char **line);
bool	check_double_quote(char **line);
void	get_end(char **line);
t_token	*handle_word(char **rest, char *line);
//2_5_tokenize_check.c
bool	check_redirect_token(t_token *token);
void	check_and_sort_tokens(t_token **head);

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
int  ft_strcmp(char* str1, char* str2);

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

//6_6_1_export_builtin.c
void    export_builtin(t_info *info, t_environ *list);
//6_6_2_export_builtin.c
int	    ft_strchr_index(const char *str, char c);
void	update_env(char *arg, t_environ *list);
void	add_new_env(char *arg, t_environ *list);
//6_6_3_export_builtin.c
bool	check_argv_no_such_env(t_info *info);
bool	check_add_or_not(char *arg, t_environ *list);
void	add_env_value(char *arg, t_environ *list);
int     not_allowed_variant_character(char *key);

//unset_builtin.c
void    unset_builtin(t_info *info, t_environ *list);

//utils.c
char    **x_double_str_malloc(int num);
char    *x_str_malloc(int num);
char    *x_strdup(char *str);
int    **x_double_int_malloc(int num);
int    *x_int_malloc(int num);

void	write_to_heredoc_one(char **array, int i, t_environ *list);
void	write_to_heredoc_not_one(char **array, int i, int *heredoc_flag, t_environ *list);
void	just_append(t_token *tok, char **p, char **new_word);
void	expand_variable(char **p, char **new_word, t_environ *list);
void	remove_double_quote(char **p, char **new_word, t_environ *list);
void	remove_single_quote(char **p, char **new_word);
void	remove_double_quote(char **p, char **new_word, t_environ *list);
char	*double_variable_expand(char *new_word, t_environ *list);
void	append_char(char **s, char c);
int	    is_variable_character(char c);
char	**make_path_list(t_environ *list);
int	    *place_pipe(t_info *info);
void	safty_free(char **str);
int     check_builtin(char *argv);
void	make_info_argv(t_info *info, int end, int start);
int	    check_redirect(t_info *info);
void	do_builtin(char *argv, t_environ *list, t_info *info);
void	multiple_pipes(t_info *info, t_environ *list);
void	remove_redirect(char **tmp, t_info *info, int i);
int	branch_do_v2(t_info *info, int i, int heredoc_count);
int	branch_do_v1(t_info *info, int i, int heredoc_count);
void	do_input(t_info *info, int i);
void	do_output(t_info *info, int i);
void	do_heredoc_one(t_info *info, int i);
void	do_heredoc_not_one(t_info *info, int i);
void	do_append(t_info *info, int i);
int	detect_something(t_info *info, int i);
int	count_splitable(t_info *info, int start, int end);
char	*make_exepath(char *path, char *command);
void	do_fd(t_info *info, int i);
void	execute_check_equal(t_info *info, int j);
void	check_equal(t_info *info, int *j);
void	do_mix_builtin(t_info *info, t_environ *list, int i);
char	*create_path(t_info *info, int j);
void	malloc_error_free(int **pipefd, int i);
void	check_variable_character(char *var, char **p);

#endif