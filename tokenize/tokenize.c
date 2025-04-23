/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:55:33 by fdurban-          #+#    #+#             */
/*   Updated: 2025/04/23 13:20:16 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_special(char letter)
{
	if(letter == '>' || letter == '<' || letter == '|')
		return (1);
	else
		return (0);
}

char	*extract_word(char *str, int *i)
{
	int start = *i;
	while (str[*i] && !isspace(str[*i]) && !is_special(str[*i]))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

char	*extract_quoted_string(char *str, int *i)
{
	int		start;
	char	quote;

	(*i)++;
	start = *i;
	quote = str[*i];
	while (str[*i] && str[*i] != quote)
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

void	add_command_part_to_list(t_command_part **lst, t_command_part *new)
{
	t_command_part	*last;

	last = *lst;
	if (!new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	if (lst != NULL)
	{
		while (last->next != NULL)
			last = last->next;
		last->next = new;
	}
}

t_command_part		*create_command_part(char *value, t_input type)
{
	t_command_part *new;
	
	new = malloc(sizeof(t_command_part));
	new->value = value;
	new->type = type;
	return (new);
}
//char command
char	**tokenize(char *valid_command, int token_number)
{
	char	**tokens;
	t_input	input;
	int		i;
	t_word_type word_type;

	i = 0;
	tokens = ft_split(valid_command, '|');
	//char			***parsed_tokens;
	//pipe // space //letter // end // single quote //double quote //redirect
	const int	matrix[NUM_WORDS][NUM_INPUT] = {
	{WORD_ERROR, WORD_START, WORD_STANDARD, WORD_END, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_REDIRECT_IN}, //WORD_START
	{WORD_ERROR, WORD_SPACE, WORD_STANDARD, WORD_END, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_REDIRECT_IN}, // WORD_STANDARD
	{WORD_ERROR, WORD_SINGLE_QUOTE, WORD_SINGLE_QUOTE, WORD_ERROR, WORD_END_OF_SINGLE_QUOTE,WORD_SINGLE_QUOTE, WORD_SINGLE_QUOTE}, // WORD_SINGLE QUOTE
	{WORD_ERROR, WORD_DOUBLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_ERROR, WORD_DOUBLE_QUOTE, WORD, END_OF_DOUBLE_QUOTE, WORD_DOUBLE_QUOTE}, // WORD_DOUBLE QUOTE
	{WORD_ERROR, WORD_SPACE, WORD_STANDARD, WORD_ERROR, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_HEREDOC}, // REDIRECT_IN
	{WORD_ERROR, WORD_REDIRECT_OUT, WORD_REDIRECT_OUT, WORD_END, WORD_REDIRECT_OUT, WORD_REDIRECT_OUT, WORD_REDIRECT_APPEND}, // REDIRECT_OUT
	{WORD_ERROR, }, // REDIRECT_APPEND
	{WORD_ERROR, }, // HERE_DOC
	{WORD_ERROR, WORD_SPACE, WORD_STANDARD, WORD_END, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_REDIRECT_IN}, // END OF SINGLE QUOTE
	{WORD_ERROR, }, // END OF DOUBLE QUOTE
	{WORD_ERROR, } // SPACE
	};
	word_type = WORD_START;

	printf("%d\n", token_number);
	//sustituir por funciones que partan las diferentes partes del pipe y lo ponga en una lista enlazada
	while(1)
	{
		input = get_input_type(valid_command[i]);
		if((word_type = WORD_ERROR) || (word_type == WORD_END))
			break;
		word_type = matrix[word_type][input];	
		i++;
	}
	//parsed_tokens =  malloc(sizeof(char **) * token_number + 1);
	return (tokens);
}