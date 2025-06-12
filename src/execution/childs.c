/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 11:35:38 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/11 17:59:17 by igngonza         ###   ########.fr       */
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

static void	setup_child_redirection(t_pipex *px)
{
	int	idx;

	idx = px->idx;
	if (idx == 0)
	{
		dup2(px->pipes[1], STDOUT_FILENO);
	}
	else if (idx == px->cmd_count - 1)
	{
		dup2(px->pipes[2 * (idx - 1)], STDIN_FILENO);
	}
	else
	{
		dup2(px->pipes[2 * (idx - 1)], STDIN_FILENO);
		dup2(px->pipes[2 * idx + 1], STDOUT_FILENO);
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

// static void	launch_command(char **cmd, char *path, t_shell *shell)
//{
//	execve(path, cmd, shell->env->vars);
//	print_exec_error_and_exit(path);
//}

void	create_child_process(t_pipex *px, t_shell *shell)
{
	pid_t pid = fork();
	if (pid < 0)
		handle_error("fork failed");
	px->pids[px->idx] = pid;

	if (pid == 0)
	{
		char **cmd;
		char *path;

		/* Restore default signal handling in the child */
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);

		/* Apply file-based redirections */
		if (px->in_fd >= 0)
		{
			if (dup2(px->in_fd, STDIN_FILENO) < 0)
				handle_error("dup2 infile");
			close(px->in_fd);
		}
		if (px->out_fd >= 0)
		{
			if (dup2(px->out_fd, STDOUT_FILENO) < 0)
				handle_error("dup2 outfile");
			close(px->out_fd);
		}

		/* Pipe redirections (only if a pipeline) */
		if (px->cmd_count > 1)
			setup_child_redirection(px);

		/* Close all pipe fds */
		close_pipes(px);

		/* Execute the command */
		cmd = px->cmd_args[px->idx];
		if (is_builtin(cmd[0]))
			exit(exec_builtin(cmd, shell));

		path = get_executable_path(px, cmd[0]);
		execve(path, cmd, shell->env->vars);
		print_exec_error_and_exit(path);
	}
	else
	{
		if (px->cmd_count > 1)
			cleanup_parent_pipes(px);
	}
}