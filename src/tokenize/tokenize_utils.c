/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:03:53 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/19 12:28:05 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_pipe_at_end(char *cmd, int i)
{
	int	j;

	if (cmd[i] != '|')
		return (0);
	j = i + 1;
	while (cmd[j] && cmd[j] == ' ')
		j++;
	if (cmd[j] == '\0')
	{
		printf("Syntax error: unexpected end after pipe\n");
		return (1);
	}
	return (0);
}

int	validate_command_syntax(char *valid_command,
	const int matrix[W_TOTAL][I_NUM_INPUT])
{
	int	i;
	int	state;
	int	input;

	i = 0;
	state = W_START;
	while (state != W___END)
	{
		if (is_pipe_at_end(valid_command, i))
			return (1);
		input = get_token_type(valid_command[i]);
		state = matrix[state][input];
		if (state == W_ERROR)
		{
			printf("Syntax Error\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	count_segments(const char *cmd, const int matrix[W_TOTAL][I_NUM_INPUT])
{
	int	i;
	int	state;
	int	count;
	int	input;

	state = W_START;
	count = 1;
	i = 0;
	while (cmd[i])
	{
		input = get_token_type(cmd[i]);
		state = matrix[state][input];
		i++;
		if (state == W_EPIPE)
			count++;
	}
	return (count);
}

void	fill_segments(char **segments, char *cmd,
		const int matrix[W_TOTAL][I_NUM_INPUT])
{
	int	i;
	int	state;
	int	start;
	int	segment_index;
	int	input;

	i = 0;
	state = W_START;
	start = 0;
	segment_index = 0;
	while (cmd[i])
	{
		input = get_token_type(cmd[i]);
		state = matrix[state][input];
		i++;
		if (state == W___END || cmd[i] == '\0' || state == W_EPIPE)
		{
			segments[segment_index++] = ft_substr(cmd, start, i - start);
			start = i;
			state = W_START;
		}
	}
	segments[segment_index] = NULL;
}

void	free_command_part_list(t_command_part *lst)
{
	t_command_part	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst->value);
		free(lst);
		lst = tmp;
	}
}
