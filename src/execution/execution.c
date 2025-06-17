/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:10:55 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/17 12:54:18 by igngonza         ###   ########.fr       */
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

// static int	open_input_file(char *filename)
//{
//	int	fd;

//	fd = open(filename, O_RDONLY);
//	if (fd < 0)
//		fprintf(stderr, "mini: %s: No such file or directory\n", filename);
//	return (fd);
//}

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
		if (p->type == W_STNDR || p->type == W_SINGQ || p->type == W_DOUBQ)
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
		// Skip redirection tokens and their targets (no opening here!)
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

	cmd_count = count_command_segments(segs);
	px->cmd_count = cmd_count;
	px->cmd_args = malloc(sizeof(char **) * (cmd_count + 1));
	if (!px->cmd_args)
		handle_error("malloc cmd_args");
	for (int i = 0; i < cmd_count; i++)
		process_segment(px, segs[i], i);
	px->cmd_args[cmd_count] = NULL;
}

int	execution(t_command_part **cmd_segs, t_shell *shell)
{
	t_pipex	px;
	pid_t	last_pid;
	int		status;

	ft_bzero(&px, sizeof(px));
	px.in_fd = -1;
	px.out_fd = -1;
	px.here_doc = 0;
	px.redir_failures = malloc(sizeof(int) * px.cmd_count);
	if (!px.redir_failures)
		handle_error("malloc redir_failures");
	ft_bzero(px.redir_failures, sizeof(int) * px.cmd_count);
	parse_cmds_from_tokens(&px, cmd_segs);
	px.cmd_segs = cmd_segs;
	if (px.cmd_count == 0)
	{
		cleanup_pipex(&px);
		shell->exit_status = 1;
		return (1);
	}
	parse_paths(&px, shell);
	if (px.cmd_count == 1 && is_builtin(px.cmd_args[0][0]))
		return (exec_builtin(px.cmd_args[0], shell));
	last_pid = spawn_pipeline(&px, shell);
	status = collect_status(last_pid);
	cleanup_pipex(&px);
	shell->exit_status = status;
	return (status);
}
