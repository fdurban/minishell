/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 10:06:17 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/25 19:25:28 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	open_redirection_fd(t_command_part *node, t_pipex *px,
		t_shell *shell)
{
	char	*path;

	(void)px;
	(void)shell;
	path = node->next->value;
	if (node->type == W_REDIN)
		return (open(path, O_RDONLY));
	else if (node->type == W_REDOU)
		return (open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644));
	else if (node->type == W_REDAP)
		return (open(path, O_CREAT | O_WRONLY | O_APPEND, 0644));
	else if (node->type == W_HRDOC)
	{
		if (!px->heredoc_filename)
			return (-1);
		return (open(px->heredoc_filename, O_RDONLY));
	}
	return (-1);
}

static void	apply_fd_redirection(int fd, int type)
{
	if (type == W_REDIN || type == W_HRDOC)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
}

void	handle_redirections(t_pipex *px, t_shell *shell)
{
	t_command_part	*node;
	int				fd;

	node = px->cmd_segs[px->idx];
	while (node)
	{
		if ((node->type == W_REDIN || node->type == W_REDOU
				|| node->type == W_REDAP || node->type == W_HRDOC)
			&& node->next)
		{
			fd = open_redirection_fd(node, px, shell);
			if (fd < 0)
			{
				handle_redirection_error(node->next->value);
				exit(1);
			}
			apply_fd_redirection(fd, node->type);
			close(fd);
			node = node->next;
		}
		node = node->next;
	}
}

void	execute_child_command(t_pipex *px, t_env *envp)
{
	char	*cmd;
	char	**args;

	cmd = px->cmd_paths[px->idx];
	args = px->cmd_args[px->idx];
	if (execve(cmd, args, envp->vars) == -1)
	{
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
}
