/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:24:37 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/24 12:30:46 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	create_heredoc_file(void)
{
	int	fd;

	fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		handle_error(ERR_HEREDOC);
	return (fd);
}

void	process_heredoc_input(char *limiter, int fd)
{
	char	*buf;
	size_t	lim_len;

	lim_len = ft_strlen(limiter);
	while (1)
	{
		write(1, "heredoc> ", 9);
		buf = get_next_line(STDIN_FILENO);
		if (!buf)
			break ;
		if (!ft_strncmp(limiter, buf, lim_len) && buf[lim_len] == '\n')
		{
			free(buf);
			break ;
		}
		write(fd, buf, ft_strlen(buf));
		free(buf);
	}
}

void	finalize_heredoc(t_pipex *pipex)
{
	pipex->in_fd = open(".heredoc_tmp", O_RDONLY);
	if (pipex->in_fd < 0)
	{
		unlink(".heredoc_tmp");
		handle_error(ERR_HEREDOC);
	}
}

void	handle_heredoc(char *limiter, t_pipex *pipex)
{
	pid_t	pid;
	int		status;
	int		fd;

	pid = fork();
	if (pid == -1)
		handle_error("heredoc: fork failed");
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		g_signal_state = 2;
		fd = create_heredoc_file();
		process_heredoc_input(limiter, fd);
		close(fd);
		exit(0);
	}
	else
	{
		waitpid(pid, &status, 0);
		g_signal_state = 0;
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			unlink(".heredoc_tmp");
			write(1, "\n", 1);
			exit(1);
		}
		finalize_heredoc(pipex);
	}
}
