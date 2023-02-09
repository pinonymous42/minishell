/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   step_5_6.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 16:18:59 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/09 12:09:41 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STEP_5_6_H
# define STEP_5_6_H

# include <stdbool.h>
# include "step_1_4.h"

//error.c
void	fatal_error(const char *msg) __attribute__((noreturn));
void	assert_error(const char *msg) __attribute__((noreturn));
void	err_exit(const char *location, const char *msg, int status) __attribute__((noreturn));

//tokenize.c

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

t_token *tokenize(char *line);
char	**token_list_to_argv(t_token *token);

// destructor.c
void free_all_token(t_token *token);
void free_argv(char **argv);

#endif