/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:55:33 by fdurban-          #+#    #+#             */
/*   Updated: 2025/05/15 15:48:34 by fdurban-         ###   ########.fr       */
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
		printf("There is no type");
	new = malloc(sizeof(t_command_part));
	new->value = value;
	new->type = type;
	new->next = NULL;
	return (new);
}

char	*expand_token(t_command_part *word, t_env *env)
{
	char	*expanded_token;
	char	*result;
	char	*prefix;
	char	*tmp;
	int		i;
	int		len;
	int		start;

	i = 0;
	start = 0;
	result = ft_strdup("");
	while (word->value[i] != '\0')
	{
		start = i;
		len = 0;
		while (word->value[i] && word->value[i] != '$')
			i++;
		prefix = ft_substr(word->value, start, i - start);
		tmp = ft_strjoin(result, prefix);
		free(result);
		result = tmp;
		if(word->value[i] == '$')
		{
			start = i + 1;
			while(ft_isalnum(word->value[start + len]) || word->value[start + len] == '_')
				len++;
			expanded_token = get_env_var(env, ft_substr(word->value, start, len));
			tmp = ft_strjoin(result, expanded_token);
			free(result);
			result = tmp;
			i += len + 1;
		}
	}
	printf("El resultado de expanded token es: %s\n",result);
	return (result);
}
t_command_part	*tokenize_pipe_segment(const int matrix[W_TOTAL][NUM_INPUT], char *valid_command, t_env *env)
{
	t_input_tokenizer	input;
	int					i;
	t_word_type			word_type;
	char				*command_token;
	t_word_type			previous_word_type;
	t_command_part		*command;
	t_command_part		*lst = NULL;

	i = 0;	
	command_token = NULL;
	command = NULL;
	word_type = W_START;
	while (word_type != W___END)
	{
		previous_word_type = word_type;
		input = get_token_type(valid_command[i]);
		word_type = matrix[word_type][input];
		checkposition(word_type, valid_command, i);
		if (word_type == W_ERROR)
		{
			printf("Word type error with i loop  value of %d\n", i);
			break;
		}
		if (word_type == W___END)
			break;
		if (word_type == W_SINGQ || word_type == W_DOUBQ || word_type == W_STNDR || word_type == W_SARED || word_type == W_SPACE)
		{
			command_token = extract_token_value(valid_command, &i, matrix, word_type, previous_word_type);
			if (command_token)
			{
				command = create_command_node(command_token, word_type);
				if (word_type == W_STNDR || word_type == W_DOUBQ)
				{
					char *expanded = expand_token(command, env);
					free(command->value);
					command->value = expanded;
				}
				add_command_part_to_list(&lst, command);
			}
			continue;
		}
		i++;
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
	token = split_and_tokenize(matrix, valid_command, env);
	// print_values(token);
	return (token);
}



//probar los siguientes comandos en casa
// "abcde" abcd abcdef
// "p"w'd'



	// printf("valor de i es %d\n", *i);
	// printf("valor de start es %d\n", start);
	// printf("Numero de caracteres impresos son %d\n", *i - start);
	// i = 0;
	// while (results[i])
	// {
	// 	t_command_part *curr = results[i];
	// 	printf("Segmento %d:\n", i);
	// 	while (curr)
	// 	{
	// 	    printf("  token: %s\n", curr->value);
	// 	    curr = curr->next;
	// 	}
	// 	i++;
	// }
	// results[i] = NULL;