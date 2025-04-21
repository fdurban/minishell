/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_implementation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 13:15:03 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/21 15:49:52 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/pipex.h"

int	pipex_implementation(char **tokens, t_env *envp)
{
	t_pipex	pipex;
	int		status;
	int		last_exit_status;
	int		last_exit_id;
	int		tokens_length;

	tokens_length = envp->count;
	last_exit_status = 0;
	if (tokens_length < check_and_set_heredoc(tokens[1], &pipex))
		return (handle_msg(ERR_INPUT));
	init_files(tokens, tokens_length, &pipex);
	parse_cmds(&pipex, tokens);
	parse_paths(&pipex, envp);
	create_pipes(&pipex);
	pipex.idx = -1;
	while (++(pipex.idx) < pipex.cmd_count)
		create_child_process(&pipex, envp);
	close_pipes(&pipex);
	last_exit_id = waitpid(-1, &status, 0);
	while (last_exit_id > 0)
	{
		if (WIFEXITED(status) && pipex.pid == last_exit_id)
			last_exit_status = WEXITSTATUS(status);
		last_exit_id = waitpid(-1, &status, 0);
	}
	parent_free(&pipex);
	return (last_exit_status);
}
