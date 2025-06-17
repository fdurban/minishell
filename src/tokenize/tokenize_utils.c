/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:03:53 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/16 13:19:50 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	count_tokens(t_command_part *lst)
// {
// 	int	count;

// 	count = 0;
// 	while (lst)
// 	{
// 		count++;
// 		lst = lst->next;
// 	}
// 	return (count);
// }

// char	**tokens_to_argv(t_command_part *lst)
// {
// 	char			**argv;
// 	int				count;
// 	int				i;
// 	t_command_part	*cur;

// 	count = 0;
// 	cur = lst;
// 	while (cur)
// 	{
// 		if (cur->type != W_SPACE)
// 			count++;
// 		cur = cur->next;
// 	}
// 	argv = malloc(sizeof(char *) * (count + 1));
// 	if (!argv)
// 		return (NULL);
// 	i = 0;
// 	cur = lst;
// 	while (cur)
// 	{
// 		if (cur->type != W_SPACE)
// 			argv[i++] = ft_strdup(cur->value);
// 		cur = cur->next;
// 	}
// 	argv[i] = NULL;
// 	return (argv);
// }
