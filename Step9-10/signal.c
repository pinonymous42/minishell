/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 11:34:59 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/02/25 19:52:38 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/step_1_4.h"

int    signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        rl_on_new_line();
        rl_replace_line("", 0);
        write(1, "\n", 1);
        rl_redisplay();
    }
    return (0);
}

int set_signal()
{
    signal(SIGINT, &signal_handler);
    signal(SIGQUIT, SIG_IGN);
}