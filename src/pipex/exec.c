/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:48:20 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/21 16:06:35 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/pipex.h"

void	redirect_io(int input_fd, int output_fd)
{
	dup2(input_fd, STDIN_FILENO);
	dup2(output_fd, STDOUT_FILENO);
}

void	setup_child_io(t_pipex *pipex)
{
	if (pipex->idx == 0)
	{
		if (pipex->in_fd == -1)
		{
			parent_free(pipex);
			exit(1);
		}
		redirect_io(pipex->in_fd, pipex->pipes[1]);
	}
	else if (pipex->idx == pipex->cmd_count - 1)
	{
		if (pipex->out_fd != -1)
			redirect_io(pipex->pipes[2 * pipex->idx - 2], pipex->out_fd);
		else
		{
			parent_free(pipex);
			exit(1);
		}
	}
	else
	{
		redirect_io(pipex->pipes[2 * pipex->idx - 2], pipex->pipes[2
			* pipex->idx + 1]);
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

void	execute_child_command(t_pipex *pipex, t_env *envp)
{
	char	*cmd;
	char	**cmd_args;

	cmd = pipex->cmd_paths[pipex->idx];
	cmd_args = pipex->cmd_args[pipex->idx];
	execve(cmd, cmd_args, envp->vars);
}

void	create_child_process(t_pipex *pipex, t_env *envp)
{
	int	saved_stdout;

	pipex->pid = fork();
	if (pipex->pid == -1)
		handle_error("Fork failed");
	if (pipex->pid == 0)
	{
		saved_stdout = dup(STDOUT_FILENO);
		setup_child_io(pipex);
		close_pipes(pipex);
		handle_child_error(pipex, saved_stdout);
		execute_child_command(pipex, envp);
	}
}
