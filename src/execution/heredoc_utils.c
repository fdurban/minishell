/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:36:03 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/04 12:43:46 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_parent(char *filename, t_shell *shell, t_pipex *pipex, int i)
{
	int	status;
	int	fd;

	waitpid(-1, &status, 0);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		unlink(filename);
		free(filename);
		pipex->heredoc_filenames[i] = NULL;
		handle_error(ERR_HEREDOC);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(filename);
		free(filename);
		pipex->heredoc_filenames[i] = NULL;
		shell->exit_status = 130;
		pipex->heredoc_interrupted = 1;
	}
	else
	{
		pipex->in_fd = fd;
	}
}

void	heredoc_child(char *limiter, int type, char *filename, t_shell *shell)
{
	int	fd;

	set_signal_handlers(SHELL_HEREDOC);
	signal(SIGINT, SIG_DFL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		handle_error(ERR_HEREDOC);
	process_heredoc_input(limiter, fd, type, shell);
	close(fd);
	exit(0);
}
