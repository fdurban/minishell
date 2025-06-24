/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:21:47 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/24 17:42:44 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	cleanup_heredoc(t_pipex *pipex)
{
	if (pipex && pipex->here_doc)
	{
		unlink(pipex->heredoc_filename);
		free(pipex->heredoc_filename);
	}
}

void	safe_close_fd(int *fd)
{
	if (fd && *fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}

void	close_all_pipes(t_pipex *pipex)
{
	int	total_fds;
	int	i;

	if (!pipex)
		return ;
	total_fds = pipex->pipe_count * 2;
	i = 0;
	while (i < total_fds)
	{
		safe_close_fd(&pipex->pipes[i]);
		i++;
	}
}

void	cleanup_pipex(t_pipex *pipex)
{
	if (!pipex)
		return ;
	cleanup_heredoc(pipex);
	close_all_pipes(pipex);
	parent_free(pipex);
}
