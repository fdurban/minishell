/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retokenize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:18:05 by fdurban-          #+#    #+#             */
/*   Updated: 2025/07/19 12:00:18 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_command_part	*find_last(t_command_part *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

static void	insert_retokenized(
	t_command_part **head,
	t_command_part *prev,
	t_command_part *new_list,
	t_command_part *next)
{
	t_command_part	*last;

	last = find_last(new_list);
	if (prev)
		prev->next = new_list;
	else
		*head = new_list;
	if (last)
		last->next = next;
}

static void	process_retokenize_block(
	t_command_part **lst,
	t_command_part **tmp_ptr,
	t_command_part **prev_ptr,
	t_shell *shell)
{
	t_command_part	*next;
	t_command_part	**new_tokens;
	t_command_part	*new_list;

	new_tokens = tokenize((*tmp_ptr)->value, shell);
	new_list = *new_tokens;
	next = (*tmp_ptr)->next;
	insert_retokenized(lst, *prev_ptr, new_list, next);
	free((*tmp_ptr)->value);
	free(*tmp_ptr);
	free(new_tokens);
	*tmp_ptr = next;
}

void	retokenize(t_command_part **array, t_shell *shell)
{
	int				i;
	t_command_part	*tmp;
	t_command_part	*prev;
	t_command_part	**lst;

	i = 0;
	while (array[i])
	{
		lst = &array[i];
		tmp = *lst;
		prev = NULL;
		while (tmp)
		{
			if (tmp->needs_retokenize)
			{
				process_retokenize_block(lst, &tmp, &prev, shell);
			}
			else
			{
				prev = tmp;
				tmp = tmp->next;
			}
		}
		i++;
	}
}
