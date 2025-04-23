/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   automats.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernando <fernando@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:59:56 by fdurban-          #+#    #+#             */
/*   Updated: 2025/04/24 00:25:06 by fernando         ###   ########.fr       */
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
//TODO: Añadir estados de espacios despues de pipes y redirecciones
int	parse(char *readline)
{
	char			c;
	int				i;
	t_state			state;
	t_input_parse	input;
	int				tokens;
	const int		matrix[NUM_STATES][NUM_INPUT] = {
	{ERROR, START, WORD, END, IN_SINGLE_QUOTE, IN_DOUBLE_QUOTE, ERROR}, //START
	{PIPE, SPACE_AFTER_WORD, WORD, END, IN_SINGLE_QUOTE, IN_DOUBLE_QUOTE, END}, // STATE WORD
	{IN_SINGLE_QUOTE, IN_SINGLE_QUOTE, IN_SINGLE_QUOTE, ERROR, END_OF_SINGLE_QUOTE, IN_SINGLE_QUOTE, IN_SINGLE_QUOTE}, // STATE IN SINGLE QUOTE
	{IN_DOUBLE_QUOTE, IN_DOUBLE_QUOTE, IN_DOUBLE_QUOTE, ERROR, IN_DOUBLE_QUOTE, END_OF_DOUBLE_QUOTE, IN_DOUBLE_QUOTE}, // STATE IN DOUBLE QUOTE
	{ERROR, SPACE_AFTER_WORD, PIPE, ERROR, IN_SINGLE_QUOTE, IN_DOUBLE_QUOTE, REDIR}, // STATE PIPE
	{ERROR, REDIR, REDIR, END, IN_SINGLE_QUOTE, IN_DOUBLE_QUOTE, REDIR}, // STATE REDIR
	{PIPE, SPACE_AFTER_WORD, WORD, END, IN_SINGLE_QUOTE, IN_DOUBLE_QUOTE, REDIR}, // SPACE AFTER WORD
	{PIPE, SPACE_AFTER_WORD, WORD, END, IN_SINGLE_QUOTE, IN_DOUBLE_QUOTE, REDIR}, // END OF SINGLE QUOTE
	{PIPE, SPACE_AFTER_WORD, WORD, END, IN_SINGLE_QUOTE, IN_DOUBLE_QUOTE, REDIR} // END OF DOUBLE QUOTE
	};
	i = 0;
	state = START;
	input = 0;
	tokens = 1;
	while (1)
	{
		c = readline[i];
		input = get_input_type(c);
		state = matrix[state][input];
		if (state == ERROR)
		{
			printf("Syntax Error\n");
			return (0);
		}
		if (state == PIPE)
			tokens++;
		if (state ==  END)
			return (tokens);
		i++;
	}
	return (tokens);
}

		// if(state == START)
		// 	printf("STATE START\n");
		// if(state == WORD)
		// 	printf("STATE WORD\n");
		// if(state == IN_SINGLE_QUOTE)
		// 	printf("STATE SINGLE QUOTE\n");	
		// if(state == IN_DOUBLE_QUOTE)
		// 	printf("STATE DOUBLE\n");	
		// if(state == REDIR)
		// 	printf("STATE REDIR\n");	
		// if(state == SPACE_AFTER_WORD)
		// 	printf("STATE SPACE AFTER WORD\n");
		// if(state == END_OF_SINGLE_QUOTE)
		// 	printf("STATE END OF SINGLE QUOTE\n");
		// if(state == END_OF_DOUBLE_QUOTE)
		// 	printf("STATE END OF DOUBLE QUOTE\n");