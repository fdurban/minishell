/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   automats.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:59:56 by fdurban-          #+#    #+#             */
/*   Updated: 2025/04/11 17:37:18 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_input_type(char c)
{
	if (c == ' ')
		return (INPUT_SPACE);
	else if (c == '\'')
		return (INPUT_IN_SINGLE_QUOTE);
	else if (c == '|')
		return (INPUT_PIPE);
	else if (c == '"')
		return (INPUT_IN_DOUBLE_QUOTE);
	else if (c == '\0')
		return (INPUT_END);
	else if (c == 'a')
		return (INPUT_LETTER);
	else
		return (0);
}

void	parse(char *readline)
{
	char		c;
	int			i;
	t_state		state;
	t_input		input;
	const int	state_matrix[NUM_STATES][NUM_INPUT] = {
	{STATE_ERROR, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}, //STATE_START
	{STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}, // STATE IN SINGLE QUOTE
	{STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}, // STATE IN DOUBLE QUOTE
	{STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}, // STATE PIPE
	{STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}, // STATE WORD
	{STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}, // STATE REDIR
	{STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END} // STATE ERROR
	};

	i = 0;
	state = STATE_START;
	//printf("%s\n", readline);
	while (state != STATE_END || state != STATE_ERROR || readline[i] != '\0')
	{
		c = readline[i];
		input = get_input_type(c);
		state = state_matrix[state][input];
		if (state == STATE_ERROR)
		{
			printf("Syntax Error\n");
			break;
		}
		i++;
	}
}
