/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Step4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:03:35 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/06 12:05:41 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "step1_4.h"

void fatal_error(const char *msg) __attribute__((noreturn));

void fatal_error(const char *msg)
{
    dprintf(STDERR_FILENO, "Fatal Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

int interpret(char *line)
{
    extern char **environ;
    char   *argv[] = {line, NULL};
    pid_t  pid;
    int    wstatus;

    pid = fork();
    if (pid < 0)
        fatal_error("fork");
    else if (pid == 0)
    {
        //子プロセスがここに入る
        execve(line, argv, environ);
        fatal_error("execve");
    }
    else
    {
        //親プロセスがここに入る
        wait(&wstatus);
        return (WEXITSTATUS(wstatus));
    }
}

int main(void)
{
    int status;
    char *line;
    
    rl_outstream = stderr;
    status = 0;
    while (1)
    {
        line = readline("minishell$");
        if (line == NULL)
            break;
        if (*line)
            add_history(line);
        status = interpret(line);
        free(line);
    }
    exit(status);
}


