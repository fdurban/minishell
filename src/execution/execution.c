/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:10:55 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/09 13:12:52 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	create_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count - 1)
	{
		if (pipe(pipex->pipes + 2 * i) < 0)
			parent_free(pipex);
		i++;
	}
}

void	init_pipex_pids(t_pipex *pipex)
{
	pipex->pids = malloc(sizeof(pid_t) * pipex->cmd_count);
	if (!pipex->pids)
		handle_error("Failed to allocate memory for pids");
}

static pid_t	spawn_pipeline(t_pipex *px, t_shell *shell)
{
	int	i;

	px->pipe_count = px->cmd_count - 1;
	px->pipes = malloc(sizeof(int) * 2 * px->pipe_count);
	if (!px->pipes)
		handle_error("Memory allocation failed for pipes");
	create_pipes(px);
	init_pipex_pids(px);
	i = 0;
	while (i < px->cmd_count)
	{
		px->idx = i;
		create_child_process(px, shell);
		i++;
	}
	close_pipes(px);
	return (px->pids[px->cmd_count - 1]);
}

static int	collect_status(pid_t last_pid)
{
	int		status;
	int		last_status;
	pid_t	pid;

	last_status = 0;
	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		pid = waitpid(-1, &status, 0);
	}
	return (last_status);
}

int	execution(char **tokens, t_shell *shell)
{
	t_pipex	px;
	pid_t	last_pid;
	int		status;

	ft_bzero(&px, sizeof(px));
	parse_cmds(&px, tokens);
	parse_paths(&px, shell);
	if (px.cmd_count == 1 && is_builtin(px.cmd_args[0][0]))
		return (exec_builtin(px.cmd_args[0], shell));
	last_pid = spawn_pipeline(&px, shell);
	status = collect_status(last_pid);
	cleanup_pipex(&px);
	shell->exit_status = status;
	return (status);
}
