/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:21:47 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/23 10:22:20 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	safe_close(int *fd)
{
	if (*fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}

void	cleanup_heredoc(t_pipex *pipex)
{
	if (pipex->here_doc)
		unlink(".heredoc_tmp");
}

void	free_cmd_paths(t_pipex *pipex)
{
	int	i;

	if (!pipex->cmd_paths)
		return ;
	i = 0;
	while (i < pipex->cmd_count)
	{
		free(pipex->cmd_paths[i]);
		pipex->cmd_paths[i] = NULL;
		i++;
	}
	free(pipex->cmd_paths);
	pipex->cmd_paths = NULL;
}

void	free_cmd_args(t_pipex *pipex)
{
	int i;
	int j;

	if (!pipex->cmd_args)
		return ;
	i = 0;
	while (i < pipex->cmd_count && pipex->cmd_args[i])
	{
		j = 0;
		while (pipex->cmd_args[i][j])
		{
			free(pipex->cmd_args[i][j]);
			pipex->cmd_args[i][j] = NULL;
			j++;
		}
		free(pipex->cmd_args[i]);
		pipex->cmd_args[i] = NULL;
		i++;
	}
	free(pipex->cmd_args);
	pipex->cmd_args = NULL;
}