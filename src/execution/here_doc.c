/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:24:37 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/14 15:55:29 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*create_heredoc_filename(void)
{
	char		*num_str;
	char		*filename;
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

static int	is_heredoc_delimiter(const char *limiter, const char *buf)
{
	size_t	lim_len;

	lim_len = ft_strlen(limiter);
	return (!ft_strncmp(limiter, buf, lim_len) && buf[lim_len] == '\n');
}

void	process_heredoc_input(char *limiter, int fd, int type, t_shell *shell)
{
	char	*buf;
	char	*tmp;

	while (1)
	{
		write(1, "heredoc> ", 9);
		buf = get_next_line(STDIN_FILENO);
		if (!buf || is_heredoc_delimiter(limiter, buf))
		{
			if (!buf)
				write(1, "\n", 1);
			free(buf);
			break ;
		}
		if (type == W_STNDR && ft_strncmp(buf, limiter, ft_strlen(limiter)))
		{
			tmp = expand_token(buf, shell);
			free(buf);
			buf = tmp;
		}
		write(fd, buf, ft_strlen(buf));
		free(buf);
	}
}

void	handle_heredoc(t_command_part *redir, t_pipex *pipex, t_shell *shell,
		int i)
{
	pid_t	pid;
	char	*filename;

	filename = create_heredoc_filename();
	pipex->heredoc_filenames[i] = filename;
	pid = fork();
	shell->state = SHELL_HEREDOC;
	if (pid == -1)
		handle_error("heredoc: fork failed");
	if (pid == 0)
		heredoc_child(redir->next->value, redir->next->type, filename, shell);
	else
		heredoc_parent(filename, shell, pipex, i);
	shell->state = SHELL_MAIN;
}
