/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 11:35:38 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/09 16:15:43 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_child_command(t_pipex *pipex, t_env *envp)
{
	char	*cmd;
	char	**cmd_args;

	cmd = pipex->cmd_paths[pipex->idx];
	cmd_args = pipex->cmd_args[pipex->idx];
	if (execve(cmd, cmd_args, envp->vars) == -1)
	{
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
}

static void	cleanup_parent_pipes(t_pipex *px)
{
	int	idx;

	idx = px->idx;
	if (idx == 0)
	{
		close(px->pipes[1]);
	}
	else if (idx == px->cmd_count - 1)
	{
		close(px->pipes[2 * (idx - 1)]);
	}
	else
	{
		close(px->pipes[2 * (idx - 1)]);
		close(px->pipes[2 * idx + 1]);
	}
}

static char	*get_executable_path(t_pipex *px, const char *raw)
{
	char	*path;

	if (ft_strchr(raw, '/'))
	{
		return ((char *)raw);
	}
	path = px->cmd_paths[px->idx];
	if (!path)
	{
		print_error_and_exit((char *)raw, "command not found", 127);
	}
	return (path);
}

static void	launch_command(char **cmd, char *path, t_shell *shell)
{
	execve(path, cmd, shell->env->vars);
	print_exec_error_and_exit(path);
}

void	create_child_process(t_pipex *px, t_shell *shell)
{
	pid_t	pid;
	char	**cmd;
	char	*raw;
	char	*path;
	int		i;

	pid = fork();
	if (pid < 0)
		handle_error("fork failed");
	px->pids[px->idx] = pid;
	if (pid == 0)
	{
		/* 1) parse and apply all file redirections for this command */
		cmd = px->cmd_args[px->idx];
		i = 0;
		// Debug before stripping
		fprintf(stderr, "[DEBUG before] cmd[%d]:", px->idx);
		for (int k = 0; cmd[k]; k++)
			fprintf(stderr, " '%s'", cmd[k]);
		fprintf(stderr, "\n");
		while (cmd[i])
			process_redirection(cmd, &i, px);
		// Debug after stripping
		fprintf(stderr, "[DEBUG after ] cmd[%d]:", px->idx);
		for (int k = 0; cmd[k]; k++)
			fprintf(stderr, " '%s'", cmd[k]);
		fprintf(stderr, "\n");
		/* 2) setup pipeline redirections if needed */
		if (px->cmd_count > 1)
			setup_child_io(px);
		/* 3) close remaining pipe fds */
		close_pipes(px);
		/* 4) execute builtin or external */
		raw = cmd[0];
		if (is_builtin(raw))
			exit(exec_builtin(cmd, shell));
		path = get_executable_path(px, raw);
		launch_command(cmd, path, shell);
	}
	else
	{
		if (px->cmd_count > 1)
			cleanup_parent_pipes(px);
	}
}
