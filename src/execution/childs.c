/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 11:35:38 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/24 14:03:08 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	cleanup_parent_pipes(t_pipex *px)
{
	int	idx;

	idx = px->idx;
	if (idx == 0)
		close(px->pipes[1]);
	else if (idx == px->cmd_count - 1)
		close(px->pipes[2 * (idx - 1)]);
	else
	{
		close(px->pipes[2 * (idx - 1)]);
		close(px->pipes[2 * idx + 1]);
	}
}

void	setup_child_pipes(t_pipex *px)
{
	if (px->cmd_count > 1)
	{
		if (px->idx > 0)
			dup2(px->pipes[2 * (px->idx - 1)], STDIN_FILENO);
		if (px->idx < px->cmd_count - 1)
			dup2(px->pipes[2 * px->idx + 1], STDOUT_FILENO);
	}
}

void	execute_command(t_pipex *px, t_shell *shell)
{
	char	**cmd;
	char	*path;

	cmd = px->cmd_args[px->idx];
	if (!cmd || !cmd[0])
	{
		if (cmd && cmd[1])
			exit(0);
		else
			exit(1);
	}
	if (is_builtin(cmd[0]))
		exit(exec_builtin(cmd, shell));
	if (ft_strchr(cmd[0], '/'))
		path = cmd[0];
	else
		path = px->cmd_paths[px->idx];
	if (!path)
		print_error_and_exit(cmd[0], "command not found", 127);
	execve(path, cmd, shell->env->vars);
	print_exec_error_and_exit(path);
}

void	create_child_process(t_pipex *px, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		handle_error("fork failed");
	px->pids[px->idx] = pid;
	if (pid == 0)
	{
		setup_child_signals();
		setup_child_pipes(px);
		handle_redirections(px, shell);
		if (px->redir_failures && px->redir_failures[px->idx])
			exit(1);
		if (!px->cmd_args || !px->cmd_args[px->idx])
			exit(1);
		close_pipes(px);
		execute_command(px, shell);
	}
	else if (px->cmd_count > 1)
		cleanup_parent_pipes(px);
}
