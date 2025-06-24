/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:10:55 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/24 11:22:07 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execution(t_command_part **cmd_segs, t_shell *shell)
{
	t_pipex	px;
	pid_t	last_pid;
	int		status;

	init_pipex(&px);
	parse_cmds_from_tokens(&px, cmd_segs);
	if (handle_empty_command(&px, shell))
		return (0);
	parse_paths(&px, shell);
	if (handle_single_builtin(&px, shell))
		return (shell->exit_status);
	last_pid = spawn_pipeline(&px, shell);
	status = collect_status(last_pid);
	cleanup_pipex(&px);
	shell->exit_status = status;
	return (status);
}
