/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 13:16:03 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/21 12:36:49 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/pipex.h"

void	get_infile(char **argv, t_pipex *pipex)
{
	if (pipex->here_doc)
		handle_heredoc(argv[2], pipex);
	else
	{
		pipex->in_fd = open(argv[1], O_RDONLY);
		if (pipex->in_fd < 0)
		{
			perror(ERR_INFILE);
			pipex->in_fd = -1;
		}
	}
}

void	get_outfile(char *argv, t_pipex *pipex)
{
	if (pipex->here_doc)
		pipex->out_fd = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0000644);
	else
		pipex->out_fd = open(argv, O_CREAT | O_RDWR | O_TRUNC, 0000644);
	if (pipex->out_fd < 0)
	{
		perror(ERR_OUTFILE);
		pipex->out_fd = -1;
	}
}

void	init_files(char **argv, int argc, t_pipex *pipex)
{
	get_infile(argv, pipex);
	get_outfile(argv[argc - 1], pipex);
	pipex->cmd_count = argc - 3 - pipex->here_doc;
	pipex->pipe_count = 2 * (pipex->cmd_count - 1);
	pipex->pipes = (int *)malloc(sizeof(int) * (pipex->pipe_count));
	if (!pipex->pipes)
		handle_error(ERR_PIPE);
}
