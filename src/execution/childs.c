/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 11:35:38 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/19 17:01:34 by igngonza         ###   ########.fr       */
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

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_child_pipes(t_pipex *px)
{
	int	rd;
	int	wr;

	if (px->cmd_count > 1)
	{
		if (px->idx > 0)
		{
			rd = px->pipes[2 * (px->idx - 1)];
			dup2(rd, STDIN_FILENO);
		}
		if (px->idx < px->cmd_count - 1)
		{
			wr = px->pipes[2 * px->idx + 1];
			dup2(wr, STDOUT_FILENO);
		}
	}
}

static int	open_redirection_fd(t_command_part *node, t_pipex *px)
{
	char	*path;
	int		fd;

	path = node->next->value;
	fd = -1;
	if (node->type == W_REDIN)
		fd = open(path, O_RDONLY);
	else if (node->type == W_REDOU)
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (node->type == W_REDAP)
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (node->type == W_HRDOC)
	{
		handle_heredoc(path, px);
		fd = open(".heredoc_tmp", O_RDONLY);
	}
	return (fd);
}

static void	apply_fd_redirection(int fd, int type)
{
	if (type == W_REDIN || type == W_HRDOC)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
}

void	handle_redirections(t_pipex *px)
{
	t_command_part	*node;
	int				fd;

	node = px->cmd_segs[px->idx];
	while (node)
	{
		if ((node->type == W_REDIN || node->type == W_REDOU
				|| node->type == W_REDAP || node->type == W_HRDOC)
			&& node->next)
		{
			fd = open_redirection_fd(node, px);
			if (fd < 0)
			{
				handle_redirection_error(node->next->value);
				exit(1);
			}
			apply_fd_redirection(fd, node->type);
			close(fd);
			node = node->next;
		}
		node = node->next;
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
			cmd++;
		else
			exit(1);
	}
	if (is_builtin(cmd[0]))
		exit(exec_builtin(cmd, shell));
	path = get_executable_path(px, cmd[0]);
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
		handle_redirections(px);
		if (px->redir_failures && px->redir_failures[px->idx])
			exit(1);
		if (!px->cmd_args || !px->cmd_args[px->idx])
			exit(1);
		close_pipes(px);
		execute_command(px, shell);
	}
	else
	{
		if (px->cmd_count > 1)
			cleanup_parent_pipes(px);
	}
}
