/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 11:43:55 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/09 12:45:42 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_commands(char **tokens)
{
	int	count;
	int	i;

	i = 0;
	count = 1;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "|") == 0)
			count++;
		i++;
	}
	return (count);
}

static char	**extract_args_for_command(char **tokens, int *pos)
{
	int		start;
	int		argc;
	char	**args;
	int		j;

	start = *pos;
	argc = 0;
	j = 0;
	while (tokens[*pos] && ft_strcmp(tokens[*pos], "|") != 0)
	{
		argc++;
		(*pos)++;
	}
	args = malloc(sizeof(char *) * (argc + 1));
	if (!args)
		handle_error("Memory allocation failed for a command");
	while (j < argc)
	{
		args[j] = ft_strdup(tokens[start + j]);
		j++;
	}
	args[argc] = NULL;
	if (tokens[*pos] && ft_strcmp(tokens[*pos], "|") == 0)
		(*pos)++;
	return (args);
}

void	parse_cmds(t_pipex *pipex, char **tokens)
{
	int	pos;
	int	n_cmds;
	int	idx;

	idx = 0;
	pos = 0;
	n_cmds = count_commands(tokens);
	pipex->cmd_count = n_cmds;
	pipex->cmd_args = malloc(sizeof(char **) * (n_cmds + 1));
	if (!pipex->cmd_args)
		handle_error("Memory allocation failed for cmd_args");
	while (idx < n_cmds)
	{
		pipex->cmd_args[idx] = extract_args_for_command(tokens, &pos);
		idx++;
	}
	pipex->cmd_args[n_cmds] = NULL;
}
