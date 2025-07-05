/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:24:37 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/05 10:08:43 by igngonza         ###   ########.fr       */
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

void	handle_heredoc(char *limiter, int type, t_pipex *pipex, t_shell *shell,
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
		heredoc_child(limiter, type, filename, shell);
	else
		heredoc_parent(filename, shell, pipex, i);
	shell->state = SHELL_MAIN;
}
