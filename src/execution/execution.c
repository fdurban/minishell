/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:10:55 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/11 17:42:49 by igngonza         ###   ########.fr       */
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
	/* 3) for each segment, build argv[] and open fds */
	i = 0;
	while (i < cmd_count)
	{
		/* first pass: count actual word tokens */
		argc = 0;
		p = segs[i];
		while (p)
		{
			if (p->type == W_STNDR || p->type == W_SINGQ || p->type == W_DOUBQ)
			{
				argc++;
			}
			else if ((p->type == W_REDIN     /* '<'  */
						|| p->type == W_REDOU  /* '>'  */
						|| p->type == W_REDAP  /* '>>' */
						|| p->type == W_HRDOC) /* '<<' */
						&& p->next)
			{
				/* handle the redirection immediately */
				if (p->type == W_REDIN)
				{
					px->in_fd = open(p->next->value, O_RDONLY);
					if (px->in_fd < 0)
					{
						fprintf(stderr, "mini: %s: No such file or directory\n",
							p->next->value);
						px->cmd_args[i] = NULL;
						px->cmd_count = i;
						return ;
					}
				}
				else if (p->type == W_REDOU)
				{
					px->out_fd = open(p->next->value,
							O_CREAT | O_WRONLY | O_TRUNC, 0644);
					if (px->out_fd < 0)
						handle_error("open outfile");
				}
				else if (p->type == W_REDAP)
				{
					px->out_fd = open(p->next->value,
							O_CREAT | O_WRONLY | O_APPEND, 0644);
					if (px->out_fd < 0)
						handle_error("open append");
				}
				else /* W_HRDOC */
				{
					px->here_doc = 1;
					handle_heredoc(p->next->value, px);
					px->in_fd = open(".heredoc_tmp", O_RDONLY);
					if (px->in_fd < 0)
						handle_error("open heredoc");
				}
				p = p->next; /* skip filename */
			}
			p = p->next;
		}
		/* allocate argv for this command */
		argv = malloc(sizeof(char *) * (argc + 1));
		if (!argv)
			handle_error("malloc argv");
		/* second pass: fill argv[], skipping redirections + filenames */
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
	/* null-terminate the array of argv pointers */
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
	parse_cmds_from_tokens(&px, cmd_segs);
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
