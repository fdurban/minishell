/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:10:55 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/19 16:59:59 by igngonza         ###   ########.fr       */
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

static int	count_command_segments(t_command_part **segs)
{
	int	count;

	count = 0;
	while (segs[count])
		count++;
	return (count);
}

static int	count_args(t_command_part *p)
{
	int	count;

	count = 0;
	while (p)
	{
		if (p->type == W_STNDR || p->type == W_SINGQ || p->type == W_DOUBQ)
			count++;
		p = p->next;
	}
	return (count);
}

static char	**build_argv(t_command_part *p, int argc)
{
	char	**argv;
	int		j;

	argv = malloc(sizeof(char *) * (argc + 1));
	j = 0;
	if (!argv)
		handle_error("malloc argv");
	while (p)
	{
		if ((p->type == W_REDIN || p->type == W_REDOU || p->type == W_REDAP
				|| p->type == W_HRDOC) && p->next)
		{
			p = p->next->next;
			continue ;
		}
		if ((p->type == W_STNDR || p->type == W_SINGQ || p->type == W_DOUBQ)
			&& p->value && p->value[0] != '\0')
			argv[j++] = p->value;
		p = p->next;
	}
	argv[j] = NULL;
	return (argv);
}

static void	process_segment(t_pipex *px, t_command_part *seg, int i)
{
	t_command_part	*p;
	int				argc;

	p = seg;
	while (p)
	{
		if ((p->type == W_REDIN || p->type == W_REDOU || p->type == W_REDAP
				|| p->type == W_HRDOC) && p->next)
			p = p->next;
		p = p->next;
	}
	argc = count_args(seg);
	px->cmd_args[i] = build_argv(seg, argc);
}

static void	parse_cmds_from_tokens(t_pipex *px, t_command_part **segs)
{
	int	cmd_count;
	int	i;
	int	j;

	cmd_count = count_command_segments(segs);
	i = 0;
	j = 0;
	px->cmd_args = malloc(sizeof(char **) * (cmd_count + 1));
	px->cmd_segs = malloc(sizeof(t_command_part *) * (cmd_count + 1));
	if (!px->cmd_args || !px->cmd_segs)
		handle_error("malloc cmd_args or cmd_segs");
	while (i < cmd_count)
	{
		process_segment(px, segs[i], j);
		if (px->cmd_args[j] && px->cmd_args[j][0])
		{
			px->cmd_segs[j] = segs[i];
			j++;
		}
		else
		{
			free(px->cmd_args[j]);
			px->cmd_args[j] = NULL;
		}
		i++;
	}
	px->cmd_args[j] = NULL;
	px->cmd_count = j;
}

int	execution(t_command_part **cmd_segs, t_shell *shell)
{
	t_pipex	px;
	pid_t	last_pid;
	int		status;
	int		saved_stdout;
	int		saved_stdin;

	ft_bzero(&px, sizeof(px));
	px.in_fd = -1;
	px.out_fd = -1;
	px.here_doc = 0;
	parse_cmds_from_tokens(&px, cmd_segs);
	px.cmd_segs = cmd_segs;
	if (px.cmd_count == 0)
	{
		cleanup_pipex(&px);
		shell->exit_status = 0;
		return (0);
	}
	parse_paths(&px, shell);
	if (px.cmd_count == 1 && is_builtin(px.cmd_args[0][0]))
	{
		saved_stdout = dup(STDOUT_FILENO);
		saved_stdin = dup(STDIN_FILENO);
		handle_redirections(&px);
		shell->exit_status = exec_builtin(px.cmd_args[0], shell);
		dup2(saved_stdout, STDOUT_FILENO);
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdout);
		close(saved_stdin);
		cleanup_pipex(&px);
		return (shell->exit_status);
	}
	last_pid = spawn_pipeline(&px, shell);
	status = collect_status(last_pid);
	cleanup_pipex(&px);
	shell->exit_status = status;
	return (status);
}
