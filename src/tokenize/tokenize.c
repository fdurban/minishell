/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:55:33 by fdurban-          #+#    #+#             */
/*   Updated: 2025/05/19 10:53:31 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// modificar en funcion de los estados

void	add_command_part_to_list(t_command_part **lst, t_command_part *new)
{
	t_command_part	*last;

	if (!new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next != NULL)
		last = last->next;
	last->next = new;
}

t_command_part	*create_command_part(char *value, t_word_type type)
{
	t_command_part	*new;

	if (!type)
		printf("There is no type");
	new = malloc(sizeof(t_command_part));
	new->value = value;
	new->type = type;
	new->next = NULL;
	return (new);
}

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

char	*extract_token_value(char *str, int *i,
		const int matrix[W_TOTAL][NUM_INPUT], t_word_type word_type,
		t_word_type previous_word_type)
{
	int					start;
	t_input_tokenizer	input;
	char				*result;
	t_word_type			end_type;

	start = *i;
	// para definir start
	if ((word_type == W_DOUBQ || word_type == W_SINGQ)
		&& (previous_word_type == W_EOSTD || previous_word_type == W_EOSTS))
		start = *i - 1;
	// para definir endtype
	if (word_type == W_DOUBQ)
		end_type = W_EOFDQ;
	else if (word_type == W_SINGQ)
		end_type = W_EOFSQ;
	else
		end_type = W_EOFST;
	// para sacar resultado
	if (word_type == W_SPACE || word_type == W_SARED)
	{
		start = *i;
		while (word_type == W_SPACE || word_type == W_SARED)
		{
			(*i)++;
			input = get_token_type(str[*i]);
			word_type = matrix[word_type][input];
		}
		result = ft_substr(str, start, *i - start);
		if (!result)
		{
			printf("Error extrayendo espacios\n");
			return (NULL);
		}
		return (result);
	}
	while (word_type == W_DOUBQ || word_type == W_SINGQ || word_type == W_STNDR)
	{
		(*i)++;
		input = get_token_type(str[*i]);
		word_type = matrix[word_type][input];
	}
	if (end_type == W_EOFDQ || end_type == W_EOFSQ)
		result = ft_substr(str, start + 1, *i - start - 1);
	else
		result = ft_substr(str, start, *i - start);
	if (!result)
	{
		printf("Error\n");
		return (NULL);
	}
	return (result);
}
t_command_part	*tokenize_pipe_segment(const int matrix[W_TOTAL][NUM_INPUT],
		char *valid_command)
{
	t_input_tokenizer	input;
	int					i;
	t_word_type			word_type;
	char				*random_command;
	t_word_type			previous_word_type;
	t_command_part		*command;
	t_command_part		*lst;

	lst = NULL;
	i = 0;
	random_command = NULL;
	command = NULL;
	word_type = W_START;
	while (valid_command[i] != '\0' && word_type != W___END)
	{
		previous_word_type = word_type;
		input = get_token_type(valid_command[i]);
		word_type = matrix[word_type][input];
		// checkposition(word_type, valid_command, i);
		if (word_type == W_ERROR)
		{
			printf("Word type error with i loop  value of %d\n", i);
			break ;
		}
		if (word_type == W___END)
			break ;
		if (word_type == W_SINGQ || word_type == W_DOUBQ || word_type == W_STNDR
			|| word_type == W_SARED || word_type == W_SPACE)
		{
			random_command = extract_token_value(valid_command, &i, matrix,
					word_type, previous_word_type);
			if (random_command)
			{
				command = create_command_part(random_command, word_type);
				add_command_part_to_list(&lst, command);
			}
			// printf("random command is %s\n", random_command);
			// printf("Salto del largo de la cadena %d\n", i);
			continue ;
		}
		i++;
	}
	return (lst);
}
t_command_part	**split_and_tokenize(const int matrix[W_TOTAL][NUM_INPUT],
		char *valid_command)
{
	char			**tokens;
	int				count;
	int				i;
	t_command_part	**results;

	count = 0;
	i = 0;
	tokens = ft_split(valid_command, '|');
	if (!tokens)
		return (NULL);
	while (tokens[count])
		count++;
	results = malloc(sizeof(t_command_part *) * (count + 1));
	while (tokens[i])
	{
		results[i] = tokenize_pipe_segment(matrix, tokens[i]);
		i++;
	}
	results[i] = NULL;
	return (results);
}

t_command_part	**tokenize(char *valid_command)
{
	t_command_part	**token;

	const int matrix[W_TOTAL][NUM_INPUT] = {
		{W_START, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU},
		// W_START
		{W_SPACE, W_STNDR, W___END, W_EOSTS, W_EOSTD, W_EOFST, W_EOFST},
		// W_STNDR
		{W_SINGQ, W_SINGQ, W_ERROR, W_EOFSQ, W_SINGQ, W_SINGQ, W_SINGQ},
		// WORD_SINGLE QUOTE
		{W_DOUBQ, W_DOUBQ, W_STNDR, W_DOUBQ, W_EOFDQ, W_DOUBQ, W_DOUBQ},
		// WORD_DOUBLE QUOTE
		{W_SPACE, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_HRDOC, W_ERROR},
		// REDIRECT_IN
		{W_REDOU, W_REDOU, W___END, W_REDOU, W_REDOU, W_REDIN, W_REDAP},
		// REDIRECT_OUT
		{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR},
		// REDIRECT_APPEND
		{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR},
		// HERE_DOC
		{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU},
		// SPACE AFTER WORD
		{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR},
		// SPACE AFTER REDIRECT
		{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU},
		// END OF SINGLE QUOTE
		{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU},
		// END OF DOUBLE QUOTE
		{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU},
		// END OF STANDARD
		{W_DOUBQ, W_DOUBQ, W___END, W_EOFDQ, W_EOFDQ, W_DOUBQ, W_DOUBQ},
		// END OF STANDARD TO DOUBLE QUOTE
		{W_SINGQ, W_SINGQ, W___END, W_EOFSQ, W_SINGQ, W_SINGQ, W_SINGQ}
		// END OF STANDARD TO DOUBLE QUOTE
	};
	token = split_and_tokenize(matrix, valid_command);
	if (!token)
	{
		printf("[DEBUG] Tokenization returned NULL.\n");
		return (NULL);
	}
	return (token);
}

// probar los siguientes comandos en casa
// "abcde" abcd abcdef
// "p"w'd'

// printf("valor de i es %d\n", *i);
// printf("valor de start es %d\n", start);
// printf("Numero de caracteres impresos son %d\n", *i - start);