/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:41:35 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/24 14:00:12 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_pipex(t_pipex *px)
{
	ft_bzero(px, sizeof(t_pipex));
	px->in_fd = -1;
	px->out_fd = -1;
	px->here_doc = 0;
}

int	handle_empty_command(t_pipex *px, t_shell *shell)
{
	if (px->cmd_count == 0)
	{
		cleanup_pipex(px);
		shell->exit_status = 0;
		return (1);
	}
	return (0);
}

int	handle_single_builtin(t_pipex *px, t_shell *shell)
{
	int	saved_stdout;
	int	saved_stdin;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (px->cmd_count == 1 && is_builtin(px->cmd_args[0][0]))
	{
		handle_redirections(px, shell);
		shell->exit_status = exec_builtin(px->cmd_args[0], shell);
		dup2(saved_stdout, STDOUT_FILENO);
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdout);
		close(saved_stdin);
		cleanup_pipex(px);
		return (1);
	}
	close(saved_stdout);
	close(saved_stdin);
	return (0);
}
