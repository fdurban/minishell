/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakul <yakul@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 11:43:55 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/15 15:38:01 by yakul            ###   ########.fr       */
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
			handle_heredoc(p->next, px, shell, i);
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

static void	process_and_filter_segment(t_pipex *px, t_command_part *seg,
		int index, t_shell *shell)
{
	px->heredoc_filenames[index] = NULL;
	process_segment(px, seg, index, shell);
	if (px->cmd_args[index] && px->cmd_args[index][0])
		px->cmd_segs[index] = seg;
	else
	{
		free(px->cmd_args[index]);
		px->cmd_args[index] = NULL;
	}
}

void	parse_cmds_from_tokens(t_pipex *px, t_command_part **segs,
		t_shell *shell)
{
	int	cmd_count;
	int	i;
	int	j;

	cmd_count = count_command_segments(segs);
	allocate_pipex_structs(px, cmd_count);
	i = 0;
	j = 0;
	while (i < cmd_count)
	{
		process_and_filter_segment(px, segs[i], j, shell);
		if (px->cmd_args[j] && px->cmd_args[j][0])
			j++;
		i++;
	}
	px->cmd_args[j] = NULL;
	px->cmd_count = j;
}
