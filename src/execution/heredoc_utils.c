/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:36:03 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/01 19:57:08 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_parent(t_pipex *pipex, t_shell *shell)
{
	int	status;

	waitpid(-1, &status, 0);
	g_signal_state = 0;
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(pipex->heredoc_filename);
		free(pipex->heredoc_filename);
		pipex->heredoc_filename = NULL;
		shell->exit_status = 130;
		pipex->heredoc_interrupted = 1;
	}
	else
		finalize_heredoc(pipex);
}

void	heredoc_child(char *limiter, int type, t_pipex *pipex, t_shell *shell)
{
	int	fd;

	set_signal_handlers(SHELL_HEREDOC);
	signal(SIGINT, SIG_DFL);
	g_signal_state = 2;
	fd = create_heredoc_file(pipex);
	process_heredoc_input(limiter, fd, type, shell);
	close(fd);
	exit(0);
}
