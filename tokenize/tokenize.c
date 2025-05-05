/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:55:33 by fdurban-          #+#    #+#             */
/*   Updated: 2025/05/05 16:55:48 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/tokenizer.h"
#include "../includes/minishell.h"

//modificar en funcion de los estados

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
	t_word_type			end_type;

	start = i;
	printf("El valor de start es %d\n", start);
	if ((word_type == WORD_DOUBLE_QUOTE))
		end_type = WORD_END_OF_DOUBLE_QUOTE;
	else if (word_type == WORD_SINGLE_QUOTE)
		end_type = WORD_END_OF_SINGLE_QUOTE;
	else
		end_type = WORD_END_STANDARD;
	while (word_type != end_type && word_type != WORD_ERROR && word_type != WORD_END)
	{
		input = get_token_type(str[i]);
		word_type = matrix[word_type][input];
		i++;
	}
	result = ft_substr(str, start, i - start - 1);
	printf("valor de i es %d\n", i);
	printf("valor de start es %d\n", start);
	printf("Numero de caracteres impresos son %d\n", i - start - 1);
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
	{WORD_END_STANDARD, WORD_STANDARD, WORD_END, WORD_END_STANDARD, WORD_END_STANDARD, WORD_END_STANDARD, WORD_END_STANDARD}, // WORD_STANDARD
	{WORD_SINGLE_QUOTE, WORD_SINGLE_QUOTE, WORD_ERROR, WORD_END_OF_SINGLE_QUOTE,WORD_SINGLE_QUOTE, WORD_SINGLE_QUOTE}, // WORD_SINGLE QUOTE
	{WORD_DOUBLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_STANDARD, WORD_DOUBLE_QUOTE, WORD_END_OF_DOUBLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_DOUBLE_QUOTE}, // WORD_DOUBLE QUOTE
	{WORD_SPACE, WORD_STANDARD, WORD_ERROR, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_HEREDOC}, // REDIRECT_IN
	{WORD_REDIRECT_OUT, WORD_REDIRECT_OUT, WORD_END, WORD_REDIRECT_OUT, WORD_REDIRECT_OUT,WORD_REDIRECT_IN, WORD_REDIRECT_APPEND}, // REDIRECT_OUT
	{WORD_SPACE_AFTER_REDIRECT, WORD_STANDARD, WORD_ERROR, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_ERROR}, // REDIRECT_APPEND
	{WORD_SPACE_AFTER_REDIRECT, WORD_STANDARD, WORD_ERROR, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_ERROR}, // HERE_DOC
	{WORD_SPACE, WORD_STANDARD, WORD_END, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_REDIRECT_IN, WORD_REDIRECT_OUT}, // SPACE AFTER WORD
	{WORD_SPACE_AFTER_REDIRECT, WORD_STANDARD, WORD_ERROR, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_ERROR, WORD_ERROR}, // SPACE AFTER REDIRECT
	{WORD_SPACE, WORD_STANDARD, WORD_END, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_REDIRECT_IN, WORD_REDIRECT_OUT}, // END OF SINGLE QUOTE
	{WORD_SPACE, WORD_STANDARD, WORD_END, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_REDIRECT_IN, WORD_REDIRECT_OUT}, // END OF DOUBLE QUOTE
	{WORD_SPACE, WORD_STANDARD, WORD_END, WORD_SINGLE_QUOTE, WORD_DOUBLE_QUOTE, WORD_REDIRECT_IN, WORD_REDIRECT_OUT}// END OF STANDARD
};
word_type = WORD_START;
//sustituir por funciones que partan las diferentes partes del pipe y lo ponga en una lista enlazada
	while (valid_command[i] != '\0' && word_type != WORD_END)
	{
		input = get_token_type(valid_command[i]);
		word_type = matrix[word_type][input];
		//--------------------------------------------------------------------
		printf("---------------------------------------\n");
		printf("Letra %c\n", valid_command[i]);
		if(word_type == WORD_START)
			printf("word_type START and i value is : %d\n", i);
		if(word_type == WORD_STANDARD)
			printf("word_type WORD and i value is : %d\n", i);
		if(word_type == WORD_SINGLE_QUOTE)
			printf("word_type SINGLE QUOTE and i value is : %d\n", i);
		if(word_type == WORD_DOUBLE_QUOTE)
			printf("word_type DOUBLE and i value is : %d\n", i);
		if(word_type == WORD_REDIRECT_IN)
			printf("word_type REDIR in and i value is : %d\n", i);
		if(word_type == WORD_REDIRECT_OUT)
			printf("word_type REDIR out and i value is : %d\n", i);
		if(word_type == WORD_REDIRECT_APPEND)
			printf("word_type REDIR append and i value is : %d\n", i);
		if(word_type == WORD_SPACE)
			printf("word_type SPACE AFTER WORD and i value is : %d\n", i);
		if(word_type == WORD_SPACE_AFTER_REDIRECT)
			printf("word_type SPACE AFTER REDIRECT and i value is : %d\n", i);
		if(word_type == WORD_END_OF_SINGLE_QUOTE)
			printf("word_type END OF SINGLE QUOTE and i value is : %d\n", i);
		if(word_type == WORD_END_OF_DOUBLE_QUOTE)
			printf("word_type END OF DOUBLE QUOTE and i value is : %d\n", i);
		if(word_type == WORD_END_STANDARD)
			printf("word_type END OF STANDARD and i value is : %d\n", i);
		//----------------------------------------------------------------------------
		if((word_type == WORD_ERROR))
		{
			printf("Word type error with i loop  value of %d\n", i);
			break;
		}
		if(word_type == WORD_END)
			break;
		if (word_type == WORD_SINGLE_QUOTE || word_type == WORD_DOUBLE_QUOTE || word_type == WORD_STANDARD)
		{
			random_command = extract_command(valid_command,i, matrix, word_type);
			i+= ft_strlen(random_command);
			printf("random command is %s\n", random_command);
			printf("Salto del largo de la cadena %d\n", ft_strlen(random_command));
			continue;
		}
		i++;
	}
	printf("random command is %s\n", random_command);
	return (tokens);
}


//probar los siguientes comandos en casa
// "abcde" abcd abcdef
// "p"w'd'