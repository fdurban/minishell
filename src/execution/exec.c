/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:41:35 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/24 09:48:43 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redirect_io(int input_fd, int output_fd)
{
	dup2(input_fd, STDIN_FILENO);
	dup2(output_fd, STDOUT_FILENO);
}

void	setup_child_io(t_pipex *pipex)
{
	if (pipex->idx == 0)
	{
		dup2(pipex->pipes[1], STDOUT_FILENO);
	}
	else if (pipex->idx == pipex->cmd_count - 1)
	{
		dup2(pipex->pipes[(pipex->idx - 1) * 2], STDIN_FILENO);
	}
	else
	{
		dup2(pipex->pipes[(pipex->idx - 1) * 2], STDIN_FILENO);
		// Redirect stdout to the next pipe’s write end.
		dup2(pipex->pipes[pipex->idx * 2 + 1], STDOUT_FILENO);
	}
}

void	handle_child_error(t_pipex *pipex, int saved_stdout)
{
	if (!pipex->cmd_paths[pipex->idx])
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
		ft_printf("%s: command not found\n", pipex->cmd_args[pipex->idx][0]);
		parent_free(pipex);
		exit(127);
	}
}
