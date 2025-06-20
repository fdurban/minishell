/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 11:21:57 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/20 13:01:32 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_string_array(char **array)
{
	if (!array)
		return ;
	free(array);
}

static void	free_command_matrix(char ***matrix)
{
	int	i;

	i = 0;
	if (!matrix)
		return ;
	while (matrix[i])
	{
		free_string_array(matrix[i]);
		i++;
	}
	free(matrix);
}

void	parent_free(t_pipex *pipex)
{
	if (!pipex)
		return ;
	free_command_matrix(pipex->cmd_args);
	pipex->cmd_args = NULL;
	free_string_array(pipex->cmd_paths);
	pipex->cmd_paths = NULL;
	if (pipex->pids)
	{
		free(pipex->pids);
		pipex->pids = NULL;
	}
	if (pipex->pipes)
	{
		free(pipex->pipes);
		pipex->pipes = NULL;
	}
}

void	close_pipes(t_pipex *pipex)
{
	int	i;
	int	total_fds;

	i = 0;
	total_fds = pipex->pipe_count * 2;
	while (i < total_fds)
	{
		safe_close_fd(&pipex->pipes[i]);
		i++;
	}
}
