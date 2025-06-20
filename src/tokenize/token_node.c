/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 19:32:26 by fernando          #+#    #+#             */
/*   Updated: 2025/06/20 15:46:43 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_command_part_to_list(t_command_part **lst, t_command_part *new)
{
	t_command_part	*last;

	if (!new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next != NULL)
		last = last->next;
	last->next = new;
}

t_command_part	*create_command_node(char *value, t_word_type type)
{
	t_command_part	*new;

	if (!type)
		printf("There is no type\n");
	new = malloc(sizeof(t_command_part));
	new->value = value;
	new->type = type;
	new->next = NULL;
	return (new);
}
