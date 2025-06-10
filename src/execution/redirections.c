/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:22:50 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/10 11:19:37 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	remove_tokens(char **tokens, int idx, int count)
{
	int	j;

	j = idx;
	fprintf(stderr, "[D] remove_tokens at %d count %d; before:", idx, count);
	for (int k = 0; tokens[k]; k++)
		fprintf(stderr, " '%s'", tokens[k]);
	fprintf(stderr, "\n");
	while (tokens[j + count])
	{
		tokens[j] = tokens[j + count];
		j++;
	}
	while (count--)
		tokens[j++] = NULL;
	fprintf(stderr, "[D] after remove_tokens:");
	for (int k = 0; tokens[k]; k++)
		fprintf(stderr, " '%s'", tokens[k]);
	fprintf(stderr, "\n");
}

int	redirect_input(char *infile)
{
	int	fd;

	fprintf(stderr, "[D] redirect_input: '%s'\n", infile);
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

	fprintf(stderr, "[D] redirect_output: '%s'\n", outfile);
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

	fprintf(stderr, "[D] redirect_output_append: '%s'\n", outfile);
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

void	process_redirection(char **tokens, int *i, t_pipex *pipex)
{
	char	*tok;

	tok = tokens[*i];
	if (!tok)
		return ;
	fprintf(stderr, "[D] process_redirection at idx %d token '%s'\n", *i, tok);
	if (strcmp(tok, "<") == 0 && tokens[*i + 1])
	{
		fprintf(stderr, "[D] matched input redirection\n");
		pipex->in_fd = redirect_input(tokens[*i + 1]);
		remove_tokens(tokens, *i, 2);
		return ;
	}
	if (strcmp(tok, ">") == 0 && tokens[*i + 1])
	{
		fprintf(stderr, "[D] matched output redirection (truncate)\n");
		pipex->out_fd = redirect_output(tokens[*i + 1]);
		remove_tokens(tokens, *i, 2);
		return ;
	}
	if (strcmp(tok, ">>") == 0 && tokens[*i + 1])
	{
		fprintf(stderr, "[D] matched output redirection (append)\n");
		pipex->out_fd = redirect_output_append(tokens[*i + 1]);
		remove_tokens(tokens, *i, 2);
		return ;
	}
	if (strcmp(tok, "<<") == 0 && tokens[*i + 1])
	{
		fprintf(stderr, "[D] matched heredoc redirection\n");
		pipex->here_doc = 1;
		handle_heredoc(tokens[*i + 1], pipex);
		remove_tokens(tokens, *i, 2);
		return ;
	}
	(*i)++;
}

void	parse_redirections(t_pipex *px)
{
	int		cmd_i;
	char	**cmd;
	int		tok_i;

	cmd_i = 0;
	while ((cmd = px->cmd_args[cmd_i]))
	{
		tok_i = 0;
		while (cmd[tok_i])
			process_redirection(cmd, &tok_i, px);
		cmd_i++;
	}
	/* Debug final argv after parsing all redirections */
	for (int c = 0; px->cmd_args[c]; c++)
	{
		fprintf(stderr, "[D] cmd %d final argv:", c);
		for (int t = 0; px->cmd_args[c][t]; t++)
			fprintf(stderr, " '%s'", px->cmd_args[c][t]);
		fprintf(stderr, "\n");
	}
}
