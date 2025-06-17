/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:00:33 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/17 15:23:47 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	setup_child_redirection(t_pipex *px)
{
	int	idx;

	idx = px->idx;
	if (px->in_fd >= 0)
	{
		if (dup2(px->in_fd, STDIN_FILENO) < 0)
			handle_error("dup2 failed for input redirection");
		close(px->in_fd); // Close the original file descriptor
		px->in_fd = -1;   // Mark as closed
	}
	else if (idx > 0)
	{
		dup2(px->pipes[2 * (idx - 1)], STDIN_FILENO);
	}
	if (idx == 0 || px->in_fd >= 0)
	{
		dup2(px->pipes[1], STDOUT_FILENO);
	}
	else if (idx < px->cmd_count - 1)
	{
		dup2(px->pipes[2 * idx + 1], STDOUT_FILENO);
	}
}

void	apply_redirections(t_pipex *px)
{
	if (px->here_doc)
	{
		px->in_fd = open(".heredoc_tmp", O_RDONLY);
	}
	else if (px->in_fd >= 0)
	{
	}
	if (px->in_fd < 0)
		handle_error("open input");
	if (dup2(px->in_fd, STDIN_FILENO) < 0)
		handle_error("dup2 stdin");
	close(px->in_fd);
	if (px->out_fd >= 0)
	{
		if (dup2(px->out_fd, STDOUT_FILENO) < 0)
			handle_error("dup2 stdout");
		close(px->out_fd);
	}
}
