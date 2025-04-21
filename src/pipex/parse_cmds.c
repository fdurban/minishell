/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:18:42 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/21 12:36:55 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/pipex.h"

void	parse_cmds(t_pipex *pipex, char **argv)
{
	int	i;
	int	cmd_start;

	if (pipex->here_doc)
		cmd_start = 3;
	else
		cmd_start = 2;
	pipex->cmd_args = malloc(sizeof(char *) * (pipex->cmd_count + 1));
	if (!pipex->cmd_args)
		handle_error("Memory allocation failed for commands");
	i = 0;
	while (i < pipex->cmd_count)
	{
		pipex->cmd_args[i] = ft_split(argv[cmd_start + i], ' ');
		if (!pipex->cmd_args[i])
			handle_error("Memory allocation failed for command");
		i++;
	}
	pipex->cmd_args[i] = NULL;
}
