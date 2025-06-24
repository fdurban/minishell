/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 11:43:55 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/24 18:00:53 by igngonza         ###   ########.fr       */
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

void	process_segment(t_pipex *px, t_command_part *seg, int i)
{
	t_command_part	*p;
	int				argc;

	p = seg;
	while (p)
	{
		if (p->type == W_HRDOC && p->next != NULL)
			handle_heredoc(p->next->value, px);
		if ((p->type == W_REDIN || p->type == W_REDOU || p->type == W_REDAP)
			&& p->next)
			p = p->next;
		p = p->next;
	}
	argc = count_args(seg);
	px->cmd_args[i] = build_argv(seg, argc);
}

void	parse_cmds_from_tokens(t_pipex *px, t_command_part **segs)
{
	int	cmd_count;
	int	i;
	int	j;

	cmd_count = count_command_segments(segs);
	i = 0;
	j = 0;
	px->cmd_args = malloc(sizeof(char **) * (cmd_count + 1));
	px->cmd_segs = malloc(sizeof(t_command_part *) * (cmd_count + 1));
	if (!px->cmd_args || !px->cmd_segs)
		handle_error("malloc cmd_args or cmd_segs");
	while (i < cmd_count)
	{
		process_segment(px, segs[i], j);
		if (px->cmd_args[j] && px->cmd_args[j][0])
			px->cmd_segs[j++] = segs[i];
		else
		{
			free(px->cmd_args[j]);
			px->cmd_args[j] = NULL;
		}
		i++;
	}
	px->cmd_args[j] = NULL;
	px->cmd_count = j;
}
