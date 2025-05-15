/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:30:02 by fdurban-          #+#    #+#             */
/*   Updated: 2025/05/15 15:55:08 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/tokenizer.h"
#include "../includes/minishell.h"

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

char	*extract_space_or_sared(t_word_type word_type,int *i, char *str,  const int matrix[W_TOTAL][NUM_INPUT])
{
	int start;
	char *result;

	start = 0;
	if (word_type == W_SPACE || word_type == W_SARED)
	{
		start = *i;
		while (word_type == W_SPACE || word_type == W_SARED)
		{
			(*i)++;
			word_type = matrix[word_type][get_token_type(str[*i])];
		}
		result = ft_substr(str, start, *i - start);
		if (!result)
		{
			printf("Error extrayendo espacios\n");
			return (NULL);
		}
		return (result);
	}
	else
		return (NULL);
}

char	*extract_word(t_word_type word_type,int *i, char *str, const int matrix[W_TOTAL][NUM_INPUT], t_word_type previous_word_type)
{
	int		start;
	char	*result;

	start = *i;
	if ((word_type == W_DOUBQ || word_type == W_SINGQ) && (previous_word_type == W_EOSTD || previous_word_type == W_EOSTS))
		start = *i - 1;
	while (word_type == W_DOUBQ || word_type == W_SINGQ || word_type == W_STNDR)
	{
		(*i)++;
		word_type = matrix[word_type][get_token_type(str[*i])];
	}
	if (word_type == W_EOFDQ || word_type == W_EOFSQ)
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

char	*extract_token_value(char *str, int *i, const int matrix[W_TOTAL][NUM_INPUT], t_word_type word_type, t_word_type previous_word_type)
{
	char				*result;

	result = extract_space_or_sared(word_type, i, str, matrix);
	if (result)
		return (result);
	result = extract_word(word_type, i, str, matrix, previous_word_type);
	return (result);
}



// void	print_tokens_by_space(char ***parsed_tokens, int token_number)
// {
// 	int i = 0;
// 	while (i < token_number)
// 	{
// 		int j = 0;
// 		while(parsed_tokens[i][j])
// 		{
// 			printf("[%s]\n", parsed_tokens[i][j]);
// 			j++;
// 		}
// 		i++;
// 	}
// }