/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernando <fernando@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:55:33 by fdurban-          #+#    #+#             */
/*   Updated: 2025/05/30 23:51:47 by fernando         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/tokenizer.h"
#include "../includes/minishell.h"

//modificar en funcion de los estados

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

t_command_part	*create_command_node(char *value, t_word_type type)
{
	t_command_part	*new;

	if (!type)
		printf("There is no type\n");
	new = malloc(sizeof(t_command_part));
	new->value = ft_strdup(value);
	new->type = type;
	new->next = NULL;
	return (new);
}

void	skip_start()
{
	
}

t_command_part	*tokenize_pipe_segment(const int matrix[W_TOTAL][NUM_INPUT], char *valid_command, t_env *env)
{
	t_input_tokenizer	input;
	int					i;
	t_word_type			word_type;
	char				*command_token;
	t_word_type			previous_word_type;
	t_command_part		*command_node;
	t_command_part		*lst = NULL;
	char				*partial_token = NULL;

	i = 0;	
	command_token = NULL;
	command_node = NULL;
	word_type = W_START;
	while (word_type != W___END)
	{
		previous_word_type = word_type;
		if(word_type != W_REDIN)
		{
			input = get_token_type(valid_command[i]);
			word_type = matrix[word_type][input];
		}
		if (word_type == W_START)
		{
			i++;
			input = get_token_type(valid_command[i]);
			word_type = matrix[word_type][input];
		}
		if (word_type == W_ERROR)
		{
			printf("Error!\n");
			break;
		}
		if (word_type == W_SINGQ || word_type == W_DOUBQ || word_type == W_STNDR || word_type == W_SARED || word_type == W_SPACE || word_type == W_REDIN|| word_type == W_REDOU)
			command_token = extract_token_value(valid_command, &i, matrix, &word_type, &previous_word_type);
		else
			command_token = NULL;
		if (command_token)
		{
			command_node = create_command_node(command_token, previous_word_type);
			if (previous_word_type == W_STNDR || previous_word_type == W_DOUBQ)
			{
				char *expanded = expand_token(command_node, env);
				free(command_node->value);
				command_node->value = expanded;
			}
			if (previous_word_type == W_STNDR || previous_word_type == W_DOUBQ || previous_word_type == W_SINGQ)
			{
				if (partial_token == NULL)
					partial_token = ft_strdup(command_node->value);
				else
				{
					char	*joined = ft_strjoin(partial_token, command_node->value);
					free(partial_token);
					partial_token = joined;
				};
			}
			if ((word_type == W_SPACE || word_type == W_REDIN || word_type == W_REDOU || word_type == W___END) && partial_token)
			{
				printf("AQUí ENTRA con un valor de word_type de %d\n", word_type);
				t_command_part *joined_node = create_command_node(partial_token, previous_word_type);
				add_command_part_to_list(&lst, joined_node);
				printf("Command Added!\n");
				free(partial_token);
				partial_token = NULL;
			}
		}
	}
	return (lst);
}
t_command_part	**split_and_tokenize(const int matrix[W_TOTAL][NUM_INPUT], char *valid_command, t_env *env)
{
	char				**tokens;
	int					count;
	int					i;
	t_command_part		**results;
	
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
		results[i] = tokenize_pipe_segment(matrix, tokens[i], env);
		i++;
	}
	results[i] = NULL;
	return (results);
}

t_command_part	**tokenize(char *valid_command, t_env *env)
{
	t_command_part	**token;
	// space //letter // end // single quote //double quote //redirect IN // redirect out
	const int	matrix[W_TOTAL][NUM_INPUT] = {
	{W_START, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, //W_START
	{W_SPACE, W_STNDR, W___END, W_EOSTS, W_EOSTD, W_EOFST, W_EOFST}, // W_STNDR
	{W_SINGQ, W_SINGQ, W_ERROR, W_EOFSQ, W_SINGQ, W_SINGQ, W_SINGQ}, // WORD_SINGLE QUOTE
	{W_DOUBQ, W_DOUBQ, W_ERROR, W_DOUBQ, W_EOFDQ, W_DOUBQ, W_DOUBQ}, // WORD_DOUBLE QUOTE
	{W_SPACE, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_HRDOC, W_ERROR}, // REDIRECT_IN
	{W_SARED, W_STNDR, W___END, W_REDOU, W_REDOU, W_REDIN, W_REDAP}, // REDIRECT_OUT
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR}, // REDIRECT_APPEND
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR}, // HERE_DOC
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_EOFST, W_EOFST}, // SPACE AFTER WORD
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, // SPACE AFTER REDIRECT
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, // END OF SINGLE QUOTE
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, // END OF DOUBLE QUOTE
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, // END OF STANDARD
	{W_DOUBQ, W_DOUBQ, W___END, W_EOFDQ, W_EOFDQ, W_DOUBQ, W_DOUBQ}, // END OF STANDARD TO DOUBLE QUOTE
	{W_SINGQ, W_SINGQ, W___END, W_EOFSQ, W_SINGQ, W_SINGQ, W_SINGQ} //  END OF STANDARD TO SINGLE QUOTE
};
token = split_and_tokenize(matrix, valid_command, env);
print_values(token);
	return (token);
}
