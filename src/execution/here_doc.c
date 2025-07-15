/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:26:48 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/15 16:32:14 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_delimiter(char *buf, char *limiter, size_t lim_len)
{
	return (!ft_strncmp(limiter, buf, lim_len) && buf[lim_len] == '\n');
}

static int	should_continue_heredoc(char *buf, char *limiter, size_t lim_len)
{
	if (!buf)
	{
		write(1, "\n", 1);
		return (0);
	}
	if (is_delimiter(buf, limiter, lim_len))
	{
		free(buf);
		return (0);
	}
	return (1);
}

static void	expand_and_write(char *buf, int fd, int type, t_shell *shell)
{
	char	*tmp;

	if (type == W_STNDR)
	{
		tmp = expand_token(buf, shell);
		free(buf);
		buf = tmp;
	}
	write(fd, buf, ft_strlen(buf));
	free(buf);
}

void	process_heredoc_input(char *limiter, int fd, int type, t_shell *shell)
{
	char	*buf;
	size_t	lim_len;

	lim_len = ft_strlen(limiter);
	while (1)
	{
		write(1, "heredoc> ", 9);
		buf = get_next_line(STDIN_FILENO);
		if (!should_continue_heredoc(buf, limiter, lim_len))
			break ;
		expand_and_write(buf, fd, type, shell);
	}
}

void	handle_heredoc(t_command_part *p, t_pipex *pipex, t_shell *shell, int i)
{
	pid_t	pid;
	char	*filename;

	filename = create_heredoc_filename();
	pipex->heredoc_count++;
	pipex->heredoc_filenames[i] = filename;
	pid = fork();
	shell->state = SHELL_HEREDOC;
	if (pid == -1)
		handle_error("heredoc: fork failed");
	if (pid == 0)
		heredoc_child(p->value, p->type, filename, shell);
	else
		heredoc_parent(filename, shell, pipex, i);
	shell->state = SHELL_MAIN;
}
