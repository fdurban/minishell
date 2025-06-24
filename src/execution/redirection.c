/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernando <fernando@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:00:33 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/24 12:47:44 by fernando         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	setup_child_redirection(t_pipex *px)
{
	int	idx;

	printf("hello there");
	idx = px->idx;
	if (px->in_fd >= 0)
	{
		printf("Debug: Duplicating in_fd=%d to STDIN_FILENO\n", px->in_fd);
		if (dup2(px->in_fd, STDIN_FILENO) < 0)
			handle_error("dup2 failed for input redirection");
		close(px->in_fd); // Close the original file descriptor
		px->in_fd = -1;   // Mark as closed
	}
	else if (idx > 0)
	{
		printf("Debug: Duplicating pipe input for idx=%d\n", idx);
		dup2(px->pipes[2 * (idx - 1)], STDIN_FILENO);
	}
	if (idx == 0 || px->in_fd >= 0)
	{
		printf("Debug: Duplicating pipe output for idx=%d\n", idx);
		dup2(px->pipes[1], STDOUT_FILENO);
	}
	else if (idx < px->cmd_count - 1)
	{
		printf("Debug: Duplicating pipe output for idx=%d\n", idx);
		dup2(px->pipes[2 * idx + 1], STDOUT_FILENO);
	}
}

void	apply_redirections(t_pipex *px)
{
	char	*filename;

	filename = create_heredoc_filename();
	if (px->here_doc)
	{
		px->in_fd = open(filename, O_RDONLY);
		free(filename);
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
