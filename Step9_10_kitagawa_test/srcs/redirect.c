/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 11:55:39 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/16 23:50:49 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/step_9_10.h"

/*
<program> = <simple_command>
<simple_command> = <simple_command_element>+
<simple_command_element> = <redirect>
                         | <word>
<redirect> = <redirect_out>
           | <redirect_in>
           | <redirect_append>
           | <redirect_heredoc>

<redirect_out> = '>' <word>
<redirect_in> = '<' <word>
<redirect_append> = '>>' <word>
<redirect_heredoc> = '<<' <word>
*/

int stashfd(int fd)
{
    int stashfd;
    
    stashfd = fcntl(fd, F_DUPFD, 10);
    if (stashfd < 0)
        fatal_error("fcntl");
    if (close(fd) < 0)
        fatal_error("close");
    return (stashfd);
}

int		read_heredoc(const char *delimiter)
{
	char	*line;
	int		pfd[2];
	
	if (pipe(pfd) < 0)
		fatal_error("pipe");
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			break;
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		dprintf(pfd[1], "%s\n", line);
		free(line);
	}
	close(pfd[1]);
	return (pfd[0]);
}

int     open_redir_file(t_node *redir)
{
	if (redir == NULL)
		return (0);
	if (redir->kind == ND_REDIR_OUT)
		redir->filefd = open(redir->filename->word, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    else if (redir->kind == ND_REDIR_IN)
        redir->filefd = open(redir->filename->word, O_RDONLY);
    else if (redir->kind == ND_REDIR_APPEND)
        redir->filefd = open(redir->filename->word, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (redir->kind == ND_REDIR_HEREDOC)
		redir->filefd = read_heredoc(redir->delimiter->word);
	else
		assert_error("open_redir_file");
    if (redir->filefd < 0)
	{
		xperror(redir->filename->word);
		return (-1);
	}
	redir->filefd = stashfd(redir->filefd);
	return (open_redir_file(redir->next));
}

bool is_redirect(t_node *node)
{
	if (node->kind == ND_REDIR_OUT)
		return (true);
	else if (node->kind == ND_REDIR_IN)
		return (true);
	else if (node->kind == ND_REDIR_APPEND)
		return (true);
	else if (node->kind == ND_REDIR_HEREDOC)
		return (true);
	return (false);
}

void	do_redirect(t_node *redir)
{
	if (redir == NULL)
		return ;
	if (is_redirect(redir))
	{
		redir->stashed_targetfd = stashfd(redir->targetfd);
		dup2(redir->filefd, redir->targetfd);
	}
    else
        assert_error("do_redirect");
	do_redirect(redir->next);
}

void	reset_redirect(t_node *redir)
{
	if (redir == NULL)
		return ;
	reset_redirect(redir->next);
	if (is_redirect(redir))
	{
		close(redir->filefd);
		close(redir->targetfd);
		dup2(redir->stashed_targetfd, redir->targetfd);
	}
    else
        assert_error("reset_redirect");
}