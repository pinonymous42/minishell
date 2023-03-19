
#ifndef COPY_PIPEX_H
#define COPY_PIPEX_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "../libft/libft.h"
#include "../libgnl/get_next_line.h"
#include <fcntl.h>

#define STDIN 0
#define STDOUT 1

typedef struct s_info{
    int     input_fd;
    int     output_fd;
    int     argc;
    char    ***cmd;
    char    **path;
    char    **envp;
    int     heredoc_flag;
    // char    *limiter;
}t_info;

#endif