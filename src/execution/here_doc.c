/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:24:37 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/01 20:19:38 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*create_heredoc_filename(void)
{
	char		*num_str;
	static char	*filename;
	size_t		total_len;
	static int	counter;

	num_str = ft_itoa(counter++);
	if (!num_str)
		return (NULL);
	total_len = ft_strlen(".heredoc_") + ft_strlen(num_str) + 1;
	filename = malloc(total_len);
	if (!filename)
	{
		free(num_str);
		return (NULL);
	}
	ft_strlcpy(filename, ".heredoc_", total_len);
	ft_strlcat(filename, num_str, total_len);
	free(num_str);
	return (filename);
}

int	create_heredoc_file(t_pipex *pipex)
{
	int	fd;

	fd = open(pipex->heredoc_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		handle_error(ERR_HEREDOC);
	free(pipex->heredoc_filename);
	return (fd);
}

void	process_heredoc_input(char *limiter, int fd, int type, t_shell *shell)
{
	char	*buf;
	size_t	lim_len;
	char	*tmp;

	lim_len = ft_strlen(limiter);
	while (1)
	{
		write(1, "heredoc> ", 9);
		buf = get_next_line(STDIN_FILENO);
		if (!buf)
		{
			write(1, "\n", 1);
			break ;
		}
		if (!ft_strncmp(limiter, buf, lim_len) && buf[lim_len] == '\n')
		{
			free(buf);
			break ;
		}
		if (type == W_STNDR && ft_strncmp(buf, limiter, lim_len))
		{
			tmp = expand_token(buf, shell);
			free(buf);
			buf = tmp;
		}
		write(fd, buf, ft_strlen(buf));
		free(buf);
	}
}

void	finalize_heredoc(t_pipex *pipex)
{
	pipex->in_fd = open(pipex->heredoc_filename, O_RDONLY);
	if (pipex->in_fd < 0)
	{
		unlink(pipex->heredoc_filename);
		free(pipex->heredoc_filename);
		handle_error(ERR_HEREDOC);
	}
}

void	handle_heredoc(char *limiter, int type, t_pipex *pipex, t_shell *shell)
{
	pid_t	pid;

	if (pipex->heredoc_filename)
	{
		unlink(pipex->heredoc_filename);
		free(pipex->heredoc_filename);
		pipex->heredoc_filename = NULL;
	}
	pipex->heredoc_filename = create_heredoc_filename();
	pid = fork();
	shell->state = SHELL_HEREDOC;
	if (pid == -1)
		handle_error("heredoc: fork failed");
	if (pid == 0)
		heredoc_child(limiter, type, pipex, shell);
	else
		heredoc_parent(pipex, shell);
	shell->state = SHELL_MAIN;
}
