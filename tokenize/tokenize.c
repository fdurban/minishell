/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:55:33 by fdurban-          #+#    #+#             */
/*   Updated: 2025/05/07 16:39:01 by fdurban-         ###   ########.fr       */
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
char	*extract_command(char *str,int *i, const int matrix[W_TOTAL][NUM_INPUT], t_word_type word_type,  t_word_type previous_word_type)
{
	int					start;
	t_input_tokenizer	input;
	char				*result;
	t_word_type			end_type;

	start = *i;
	if((word_type == W_DOUBQ || word_type == W_SINGQ) && (previous_word_type == W_EOSTD || previous_word_type == W_EOSTS))
		start = *i - 1;
	if (word_type == W_DOUBQ)
		end_type = W_EOFDQ;
	else if (word_type == W_SINGQ)
		end_type = W_EOFSQ;
	else
		end_type = W_EOFST;
	while (word_type == W_DOUBQ || word_type == W_SINGQ || word_type == W_STNDR)
	{
		(*i)++;
		input = get_token_type(str[*i]);
		word_type = matrix[word_type][input];
	}
	result = ft_substr(str, start, *i - start);
	if (end_type == W_EOFDQ || end_type == W_EOFSQ)
		result = ft_substr(str, start + 1, *i - start - 1);
	return (result);
}
char	**tokenize_help(const int matrix[W_TOTAL][NUM_INPUT], char *valid_command)
{
	char				**tokens;
	t_input_tokenizer	input;
	int					i;
	t_word_type			word_type;
	char				*random_command;
	t_word_type			previous_word_type;
	t_command_part		*command;
	t_command_part		*lst = NULL;


	i = 0;	
	tokens = ft_split(valid_command, '|');
	random_command = NULL;
	command = NULL;
	// space //letter // end // single quote //double quote //redirect IN // redirect out
	word_type = W_START;
	while (valid_command[i] != '\0' && word_type != W___END)
	{
		previous_word_type = word_type;
		input = get_token_type(valid_command[i]);
		word_type = matrix[word_type][input];
		if ((word_type == W_ERROR))
		{
			printf("Word type error with i loop  value of %d\n", i);
			break;
		}
		if (word_type == W___END)
			break;
		if (word_type == W_SINGQ || word_type == W_DOUBQ || word_type == W_STNDR)
		{
			random_command = extract_command(valid_command,&i, matrix, word_type, previous_word_type);
			if (random_command)
			{
				command = create_command_part(random_command, word_type);
				add_command_part_to_list(&lst, command);
			}
			printf("random command is %s\n", random_command);
			printf("Salto del largo de la cadena %d\n", i);
			continue;
		}
		i++;
	}
	t_command_part *tmp = lst;
	while (tmp)
	{
		printf("Valor: %s, Tipo: %d\n", tmp->value, tmp->type);
		tmp = tmp->next;
	}
	return (tokens);
}

char	**tokenize(char *valid_command)
{
	const int	matrix[W_TOTAL][NUM_INPUT] = {
	{W_START, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, //W_START
	{W_EOFST, W_STNDR, W___END, W_EOSTS, W_EOSTD, W_EOFST, W_EOFST}, // W_STNDR
	{W_SINGQ, W_SINGQ, W_ERROR, W_EOFSQ, W_SINGQ, W_SINGQ, W_SINGQ}, // WORD_SINGLE QUOTE
	{W_DOUBQ, W_DOUBQ, W_STNDR, W_DOUBQ, W_EOFDQ, W_DOUBQ, W_DOUBQ}, // WORD_DOUBLE QUOTE
	{W_SPACE, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_HRDOC, W_ERROR}, // REDIRECT_IN
	{W_REDOU, W_REDOU, W___END, W_REDOU, W_REDOU, W_REDIN, W_REDAP}, // REDIRECT_OUT
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR}, // REDIRECT_APPEND
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR}, // HERE_DOC
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, // SPACE AFTER WORD
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR}, // SPACE AFTER REDIRECT
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, // END OF SINGLE QUOTE
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, // END OF DOUBLE QUOTE
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, // END OF STANDARD
	{W_DOUBQ, W_DOUBQ, W___END, W_EOFDQ, W_EOFDQ, W_DOUBQ, W_DOUBQ}, // END OF STANDARD TO DOUBLE QUOTE
	{W_SINGQ, W_SINGQ, W___END, W_EOFSQ, W_SINGQ, W_SINGQ, W_SINGQ} // END OF STANDARD TO DOUBLE QUOTE
	};

	token = tokenize_help(matrix, valid_command);
}

//probar los siguientes comandos en casa
// "abcde" abcd abcdef
// "p"w'd'
		// //--------------------------------------------------------------------
		// printf("---------------------------------------\n");
		// printf("Letra %c\n", valid_command[i]);
		// if(word_type == W_START)
		// 	printf("word_type START and i value is : %d\n", i);
		// if(word_type == W_STNDR)
		// 	printf("word_type WORD and i value is : %d\n", i);
		// if(word_type == W_SINGQ)
		// 	printf("word_type SINGLE QUOTE and i value is : %d\n", i);
		// if(word_type == W_DOUBQ)
		// 	printf("word_type DOUBLE and i value is : %d\n", i);
		// if(word_type == W_REDIN)
		// 	printf("word_type REDIR in and i value is : %d\n", i);
		// if(word_type == W_REDOU)
		// 	printf("word_type REDIR out and i value is : %d\n", i);
		// if(word_type == W_REDAP)
		// 	printf("word_type REDIR append and i value is : %d\n", i);
		// if(word_type == W_SPACE)
		// 	printf("word_type SPACE AFTER WORD and i value is : %d\n", i);
		// if(word_type == W_SARED)
		// 	printf("word_type SPACE AFTER REDIRECT and i value is : %d\n", i);
		// if(word_type == W_EOFSQ)
		// 	printf("word_type END OF SINGLE QUOTE and i value is : %d\n", i);
		// if(word_type == W_EOFDQ)
		// 	printf("word_type END OF DOUBLE QUOTE and i value is : %d\n", i);
		// if(word_type == W_EOFST)
		// 	printf("word_type END OF STANDARD and i value is : %d\n", i);
		// if(word_type == W_EOSTS)
		// 	printf("word_type END OF STANDARD TO SINGLE QUOTE and i value is : %d\n", i);
		// if(word_type == W_EOFST_TO_DOUBLE_QUOTE)
		// 	printf("word_type END OF STANDARD TO DOUBLE QUOTE and i value is : %d\n", i);
		// //----------------------------------------------------------------------------



	// printf("valor de i es %d\n", *i);
	// printf("valor de start es %d\n", start);
	// printf("Numero de caracteres impresos son %d\n", *i - start);