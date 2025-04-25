/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:55:33 by fdurban-          #+#    #+#             */
/*   Updated: 2025/04/25 18:43:12 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/tokenizer.h"
#include "../includes/minishell.h"

//modificar en funcion de los estados
int	is_special(char letter)
{
	if (letter == '>' || letter == '<' || letter == '|')
		return (1);
	else
		return (0);
}
//modificar en funcion de los estados
char	*extract_word(char *str, int *i)
{
	int start;

	start = *i;
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

t_command_part		*create_command_part(char *value, t_word_type type)
{
	t_command_part *new;
	
	new = malloc(sizeof(t_command_part));
	new->value = value;
	new->type = type;
	return (new);
}
// Este comando extrae solo comandos o partes de los comandos 
int	get_token_type(char c)
{
	if (c == ' ')
		return (TOKEN_SPACE);
	else if (c == '\'')
		return (TOKEN_IN_SINGLE_QUOTE);
	else if (c == '<')
		return (TOKEN_REDIRECT_IN);
		else if (c == '>')
			return (TOKEN_REDIRECT_OUT);
	else if (c == '"')
		return (TOKEN_IN_DOUBLE_QUOTE);
	else if (c == '\0')
		return (TOKEN_END);
	else if (c >= 33 && c <= 126)
		return (TOKEN_LETTER);
	else
		return (0);
}
//char command
char	*extract_command(char *str,int i, const int matrix[NUM_WORDS][NUM_INPUT], t_word_type word_type)
{
	int					start;
	t_input_tokenizer	input;
	char				*result;

	start = i;
	if (word_type == WORD_DOUBLE_QUOTE)
	{
		while(word_type != WORD_END_OF_DOUBLE_QUOTE && word_type != WORD_ERROR && str[i])
		{
			input = get_token_type(str[i]);
			word_type = matrix[word_type][input];
			i++;
		}
		result = ft_substr(str, start, i - start - 1);
	}
	else if (word_type == WORD_SINGLE_QUOTE)
	{
		while(word_type != WORD_END_OF_SINGLE_QUOTE && word_type != WORD_ERROR && str[i])
		{
			input = get_token_type(str[i]);
			word_type = matrix[word_type][input];
			i++;
		}
		result = ft_substr(str, start, i - start -1);
	}
	else
		result = NULL;
	return (result);
}
char	**tokenize(char *valid_command)
{
	char				**tokens;
	t_input_tokenizer	input;
	int					i;
	t_word_type			word_type;
	char				*random_command;

	i = 0;
	tokens = ft_split(valid_command, '|');
	random_command = NULL;
	// space //letter // end // single quote //double quote //redirect IN // redirect out
	const int	matrix[NUM_WORDS][NUM_INPUT] = {
	{WORD_START, WORD_STANDARD, WORD_END, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_REDIRECT_IN, WORD_REDIRECT_OUT}, //WORD_START
	{WORD_SPACE, WORD_STANDARD, WORD_END, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_REDIRECT_IN, WORD_REDIRECT_OUT}, // WORD_STANDARD
	{WORD_SINGLE_QUOTE, WORD_SINGLE_QUOTE, WORD_ERROR, WORD_END_OF_SINGLE_QUOTE,WORD_SINGLE_QUOTE, WORD_SINGLE_QUOTE}, // WORD_SINGLE QUOTE
	{WORD_DOUBLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_ERROR, WORD_DOUBLE_QUOTE, WORD_END_OF_DOUBLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_DOUBLE_QUOTE}, // WORD_DOUBLE QUOTE
	{WORD_SPACE, WORD_STANDARD, WORD_ERROR, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_HEREDOC}, // REDIRECT_IN
	{WORD_REDIRECT_OUT, WORD_REDIRECT_OUT, WORD_END, WORD_REDIRECT_OUT, WORD_REDIRECT_OUT,WORD_REDIRECT_IN, WORD_REDIRECT_APPEND}, // REDIRECT_OUT
	{WORD_SPACE_AFTER_REDIRECT, WORD_STANDARD, WORD_ERROR, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_ERROR}, // REDIRECT_APPEND
	{WORD_SPACE_AFTER_REDIRECT, WORD_STANDARD, WORD_ERROR, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_ERROR}, // HERE_DOC
	{WORD_SPACE, WORD_STANDARD, WORD_END, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_REDIRECT_IN, WORD_REDIRECT_OUT}, // SPACE AFTER WORD
	{WORD_SPACE_AFTER_REDIRECT, WORD_STANDARD, WORD_ERROR, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_ERROR, WORD_ERROR}, // SPACE AFTER REDIRECT
	{WORD_SPACE, WORD_STANDARD, WORD_END, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_REDIRECT_IN, WORD_REDIRECT_OUT}, // END OF SINGLE QUOTE
	{WORD_SPACE, WORD_STANDARD, WORD_END, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_REDIRECT_IN, WORD_REDIRECT_OUT}, // END OF DOUBLE QUOTE
	};
	word_type = WORD_START;
	//sustituir por funciones que partan las diferentes partes del pipe y lo ponga en una lista enlazada
	while (1)
	{
		input = get_token_type(valid_command[i]);
		word_type = matrix[word_type][input];	
		if((word_type == WORD_ERROR))
		{
			printf("Word type error with i loop  value of %d\n", i);
			break;
		}
		if(word_type == WORD_END)
			break;
		if (word_type == WORD_SINGLE_QUOTE || word_type == WORD_DOUBLE_QUOTE)
		{
			random_command = extract_command(valid_command,i, matrix, word_type);
			i+=ft_strlen(random_command);
		}
		printf("%s\n", random_command);
		i++;
	}
	return (tokens);
}


// if(word_type == WORD_ERROR)
// 			printf("word_type ERROR\n");
// 		if(word_type == WORD_STANDARD)
// 			printf("word_type WORD\n");
// 		if(word_type == WORD_SINGLE_QUOTE)
// 			printf("word_type SINGLE QUOTE\n");
// 		if(word_type == WORD_DOUBLE_QUOTE)
// 			printf("word_type DOUBLE\n");
// 		if(word_type == WORD_REDIRECT_IN)
// 			printf("word_type REDIR in\n");
// 		if(word_type == WORD_REDIRECT_OUT)
// 			printf("word_type REDIR out\n");
// 		if(word_type == WORD_REDIRECT_APPEND)
// 			printf("word_type REDIR append\n");
// 		if(word_type == WORD_SPACE)
// 			printf("word_type SPACE AFTER WORD\n");
// 		if(word_type == WORD_SPACE_AFTER_REDIRECT)
// 			printf("word_type SPACE AFTER REDIRECT\n");
// 		if(word_type == WORD_END_OF_SINGLE_QUOTE)
// 			printf("word_type END OF SINGLE QUOTE\n");
// 		if(word_type == WORD_END_OF_DOUBLE_QUOTE)
// 			printf("word_type END OF DOUBLE QUOTE\n");