/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 10:25:03 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/24 11:16:00 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_args(t_command_part *p)
{
	int	count;

	count = 0;
	while (p)
	{
		if (p->type == W_STNDR || p->type == W_SINGQ || p->type == W_DOUBQ)
			count++;
		p = p->next;
	}
	return (count);
}

char	**build_argv(t_command_part *p, int argc)
{
	char	**argv;
	int		j;

	j = 0;
	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		handle_error("malloc argv");
	while (p)
	{
		if ((p->type == W_REDIN || p->type == W_REDOU || p->type == W_REDAP
				|| p->type == W_HRDOC) && p->next)
		{
			p = p->next->next;
			continue ;
		}
		if ((p->type == W_STNDR || p->type == W_SINGQ || p->type == W_DOUBQ)
			&& p->value && p->value[0] != '\0')
			argv[j++] = ft_strdup(p->value);
		p = p->next;
	}
	argv[j] = NULL;
	return (argv);
}
