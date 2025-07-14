/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 11:43:55 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/14 16:24:46 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_command_segments(t_command_part **segs)
{
	int	count;

	count = 0;
	while (segs[count])
		count++;
	return (count);
}

void	process_segment(t_pipex *px, t_command_part *seg, int i, t_shell *shell)
{
	t_command_part	*p;
	int				argc;

	p = seg;
	while (p)
	{
		if (p->type == W_HRDOC && p->next != NULL)
			handle_heredoc(p, px, shell, i);
		if ((p->type == W_REDIN || p->type == W_REDOU || p->type == W_REDAP)
			&& p->next)
			p = p->next;
		p = p->next;
	}
	argc = count_args(seg);
	px->cmd_args[i] = build_argv(seg, argc);
}

static void	allocate_pipex_structs(t_pipex *px, int cmd_count)
{
	px->cmd_args = malloc(sizeof(char **) * (cmd_count + 1));
	px->cmd_segs = malloc(sizeof(t_command_part *) * (cmd_count + 1));
	px->heredoc_filenames = malloc(sizeof(char *) * (cmd_count + 1));
	if (!px->cmd_args || !px->cmd_segs || !px->heredoc_filenames)
		handle_error("malloc failed in parse_cmds_from_tokens");
}

static void	handle_command_or_cleanup(t_pipex *px, int i, int *j)
{
	if (px->cmd_args[*j] && px->cmd_args[*j][0])
		px->cmd_segs[(*j)++] = px->cmd_segs[i];
	else
	{
		free(px->cmd_args[*j]);
		px->cmd_args[*j] = NULL;
		if (px->heredoc_filenames[i])
		{
			unlink(px->heredoc_filenames[i]);
			free(px->heredoc_filenames[i]);
			px->heredoc_filenames[i] = NULL;
		}
	}
}

void	parse_cmds_from_tokens(t_pipex *px, t_command_part **segs,
		t_shell *shell)
{
	int	cmd_count;
	int	i;
	int	j;

	cmd_count = count_command_segments(segs);
	i = 0;
	j = 0;
	allocate_pipex_structs(px, cmd_count);
	while (i < cmd_count)
	{
		px->heredoc_filenames[i] = NULL;
		process_segment(px, segs[i], j, shell);
		handle_command_or_cleanup(px, i, &j);
		i++;
	}
	px->cmd_args[j] = NULL;
	px->cmd_count = j;
}
