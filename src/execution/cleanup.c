/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:21:47 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/09 12:15:13 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	cleanup_heredoc(t_pipex *pipex)
{
	if (pipex && pipex->here_doc)
		unlink(".heredoc_tmp");
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
