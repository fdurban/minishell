/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 11:21:57 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/24 14:09:11 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_string_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
		free(array[i++]);
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
	if (pipex->cmd_segs)
	{
		free(pipex->cmd_segs);
		pipex->cmd_segs = NULL;
	}
}

void	free_token_list(t_command_part *node)
{
	t_command_part	*tmp;

	while (node)
	{
		tmp = node->next;
		if (node->value)
			free(node->value);
		free(node);
		node = tmp;
	}
}

void	free_token_matrix(t_command_part **matrix)
{
	int	i;

	i = 0;
	if (!matrix)
		return ;
	while (matrix[i])
	{
		free_token_list(matrix[i]);
		i++;
	}
	free(matrix);
}
