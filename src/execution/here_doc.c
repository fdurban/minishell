/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernando <fernando@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:24:37 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/24 12:45:53 by fernando         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*create_heredoc_filename(void)
{
	char	*pid_str;
	char	*filename;
	size_t	total_len;

	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return (NULL);
	total_len = ft_strlen("/tmp/.heredoc_") + ft_strlen(pid_str) + 1;
	filename = malloc(total_len);
	if (!filename)
	{
		free(pid_str);
		return (NULL);
	}
	ft_strlcpy(filename, "/tmp/.heredoc_", total_len);
	ft_strlcat(filename, pid_str, total_len);
	free(pid_str);
	return (filename);
}

int	create_heredoc_file(void)
{
	int		fd;
	char	*filename;

	filename = create_heredoc_filename();
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		handle_error(ERR_HEREDOC);
	free(filename);
	return (fd);
}

void	process_heredoc_input(char *limiter, int type, int fd, t_shell *shell)
{
	char	*buf;
	size_t	lim_len;

	lim_len = ft_strlen(limiter);
	while (1)
	{
		write(1, "heredoc> ", 9);
		buf = get_next_line(STDIN_FILENO);
		if (type == W_STNDR)
			buf = expand_token(buf, shell);
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
	char	*filename;

	filename = create_heredoc_filename();

	pipex->in_fd = open(filename, O_RDONLY);
	if (pipex->in_fd < 0)
	{
		unlink(filename);
		free(filename);
		handle_error(ERR_HEREDOC);
	}
}

void	handle_heredoc(char *limiter, int type, t_pipex *pipex, t_shell *shell)
{
	int	fd;

	fd = create_heredoc_file();
	process_heredoc_input(limiter, type, fd, shell);
	close(fd);
	finalize_heredoc(pipex);
}
