/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   automats.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:59:56 by fdurban-          #+#    #+#             */
/*   Updated: 2025/04/14 17:41:24 by fdurban-         ###   ########.fr       */
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
	else if (c == '<' || c == '>')
		return (INPUT_REDIRECT);
	else if (c >= 33 && c <= 126)
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
	{STATE_ERROR, STATE_START, STATE_WORD, STATE_END, STATE_IN_SINGLE_QUOTE, STATE_IN_DOUBLE_QUOTE, STATE_REDIR}, //STATE_START
	{STATE_PIPE, STATE_SPACE_AFTER_WORD, STATE_WORD, STATE_END, STATE_IN_SINGLE_QUOTE, STATE_IN_DOUBLE_QUOTE, STATE_END}, // STATE WORD
	{STATE_IN_SINGLE_QUOTE, STATE_IN_SINGLE_QUOTE, STATE_IN_SINGLE_QUOTE, STATE_ERROR, STATE_START, STATE_IN_SINGLE_QUOTE, STATE_IN_SINGLE_QUOTE}, // STATE IN SINGLE QUOTE
	{STATE_IN_DOUBLE_QUOTE, STATE_IN_DOUBLE_QUOTE, STATE_IN_DOUBLE_QUOTE, STATE_ERROR, STATE_IN_DOUBLE_QUOTE, STATE_SPACE_AFTER_WORD, STATE_IN_DOUBLE_QUOTE}, // STATE IN DOUBLE QUOTE
	{STATE_ERROR, STATE_PIPE, STATE_PIPE, STATE_END, STATE_IN_SINGLE_QUOTE, STATE_IN_DOUBLE_QUOTE, STATE_REDIR}, // STATE PIPE
	{STATE_ERROR, STATE_REDIR, STATE_REDIR, STATE_ERROR, STATE_IN_SINGLE_QUOTE, STATE_IN_DOUBLE_QUOTE, STATE_REDIR}, // STATE REDIR
	{STATE_PIPE, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END} // SPACE AFTER WORD	
	};

	i = 0;
	state = STATE_START;
	while (state != STATE_END || state != STATE_ERROR || readline[i] != '\0')
	{
		printf("[%d, %d]\n", state, input);
		c = readline[i];
		input = get_input_type(c);
		state = state_matrix[state][input];
		//printf("%c\n", input);
		if (state == STATE_ERROR)
		{
			printf("Syntax Error\n");
			break;
		}
		if (state == STATE_END)
		{
			printf("Valid command\n");
			break;
		}
		i++;
	}
}
