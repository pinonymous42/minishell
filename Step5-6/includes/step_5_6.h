/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   step_5_6.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 16:18:59 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/08 20:03:58 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STEP_5_6_H
# define STEP_5_6_H

# include <stdbool.h>
# include "step_1_4.h"

typedef enum e_token_kind{
    TK_WORD,
	TK_RESERVED,
	TK_OP,
	TK_EOF,
}t_token_kind;

typedef struct s_token{
    t_token_kind    kind;
    char            *word;
    t_token         *next;
}t_token;

#endif