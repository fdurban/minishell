/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:10:55 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/10 15:35:56 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/tokenizer.h"

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

static int	single_builtin(t_pipex *px, t_shell *shell)
{
	int	status;
	int	saved_in;
	int	saved_out;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (px->in_fd >= 0)
		dup2(px->in_fd, STDIN_FILENO);
	if (px->out_fd >= 0)
		dup2(px->out_fd, STDOUT_FILENO);
	status = exec_builtin(px->cmd_args[0], shell);
	shell->exit_status = status;
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (status);
}

static void	parse_cmds_from_tokens(t_pipex *px, t_command_part **segs)
{
	int				cmd_count;
	t_command_part	*p;
	int				i;
	int				argc;
	char			**argv;
	int				j;

	cmd_count = 0;
	/* 1) count commands */
	while (segs[cmd_count])
		cmd_count++;
	px->cmd_count = cmd_count;
	/* 2) allocate cmd_args matrix */
	px->cmd_args = malloc(sizeof(char *) * (cmd_count + 1));
	if (!px->cmd_args)
		handle_error("malloc cmd_args");
	/* 3) for each segment */
	i = 0;
	while (i < cmd_count)
	{
		/* first pass: count real words */
		argc = 0;
		p = segs[i];
		while (p)
		{
			if (p->type == W_STNDR || p->type == W_SINGQ || p->type == W_DOUBQ)
			{
				argc++;
			}
			else if (p->type == W_REDIN && p->next)
			{
				/* open infile */
				px->in_fd = open(p->next->value, O_RDONLY);
				if (px->in_fd < 0)
					handle_error("open infile");
				p = p->next;
			}
			else if (p->type == W_REDOU && p->next)
			{
				px->out_fd = open(p->next->value, O_CREAT | O_WRONLY | O_TRUNC,
						0644);
				if (px->out_fd < 0)
					handle_error("open outfile");
				p = p->next;
			}
			else if (p->type == W_REDAP && p->next)
			{
				px->out_fd = open(p->next->value, O_CREAT | O_WRONLY | O_APPEND,
						0644);
				if (px->out_fd < 0)
					handle_error("open append");
				p = p->next;
			}
			else if (p->type == W_HRDOC && p->next)
			{
				px->here_doc = 1;
				handle_heredoc(p->next->value, px);
				px->in_fd = open(".heredoc_tmp", O_RDONLY);
				if (px->in_fd < 0)
					handle_error("open heredoc");
				p = p->next;
			}
			p = p->next;
		}
		/* allocate argv */
		argv = malloc(sizeof(char *) * (argc + 1));
		if (!argv)
			handle_error("malloc argv");
		/* second pass: fill argv, skipping redirects + filenames */
		j = 0;
		p = segs[i];
		while (p)
		{
			if ((p->type == W_REDIN || p->type == W_REDOU || p->type == W_REDAP
					|| p->type == W_HRDOC) && p->next)
			{
				p = p->next->next;
				continue ;
			}
			if (p->type == W_STNDR || p->type == W_SINGQ || p->type == W_DOUBQ)
			{
				argv[j++] = p->value;
			}
			p = p->next;
		}
		argv[j] = NULL;
		px->cmd_args[i++] = argv;
	}
	/* null-terminate */
	px->cmd_args[px->cmd_count] = NULL;
}
static void	debug_token_segments(t_command_part **segs)
{
	for (int cmd_i = 0; segs[cmd_i]; cmd_i++)
	{
		fprintf(stderr, "SEGMENT[%d]:\n", cmd_i);
		for (t_command_part *p = segs[cmd_i]; p; p = p->next)
		{
			fprintf(stderr, "  type=%d, value='%s'\n", p->type, p->value);
		}
	}
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
	debug_token_segments(cmd_segs);
	parse_cmds_from_tokens(&px, cmd_segs);
	parse_paths(&px, shell);
	if (px.cmd_count == 1 && is_builtin(px.cmd_args[0][0]))
	{
		status = single_builtin(&px, shell);
		cleanup_pipex(&px);
		shell->exit_status = status;
		return (status);
	}
	last_pid = spawn_pipeline(&px, shell);
	status = collect_status(last_pid);
	cleanup_pipex(&px);
	shell->exit_status = status;
	return (status);
}
