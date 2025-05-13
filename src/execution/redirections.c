/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:22:50 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/29 13:21:53 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	redirect_input(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd < 0)
	{
		perror("open infile");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2 infile");
		exit(EXIT_FAILURE);
	}
	close(fd);
	return (STDIN_FILENO);
}

int	redirect_output(char *outfile)
{
	int	fd;

	fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open outfile");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 outfile");
		exit(EXIT_FAILURE);
	}
	close(fd);
	return (STDOUT_FILENO);
}

int	redirect_output_append(char *outfile)
{
	int	fd;

	fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("open outfile (append)");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 outfile (append)");
		exit(EXIT_FAILURE);
	}
	close(fd);
	return (STDOUT_FILENO);
}
void	process_redirection(char **tokens, int *i, t_cmd *cmd)
{
	if (!tokens[*i])
		return ;
	if (ft_strcmp(tokens[*i], "<") == 0)
	{
		cmd->in_fd = redirect_input(tokens[*i + 1]);
		*i += 2;
	}
	else if (ft_strcmp(tokens[*i], ">") == 0)
	{
		cmd->out_fd = redirect_output(tokens[*i + 1]);
		*i += 2;
	}
	else if (ft_strcmp(tokens[*i], ">>") == 0)
	{
		cmd->out_fd = redirect_output_append(tokens[*i + 1]);
		*i += 2;
	}
	else if (ft_strcmp(tokens[*i], "<<") == 0)
	{
		handle_heredoc(tokens[*i + 1], cmd);
		*i += 2;
	}
}
