/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernando <fernando@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:30:02 by fdurban-          #+#    #+#             */
/*   Updated: 2025/05/30 23:48:50 by fernando         ###   ########.fr       */
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

char	*extract_space_or_sared(t_word_type *word_type, int *i, char *str, const int matrix[W_TOTAL][NUM_INPUT])
{
	int		start;
	char	*result;

	start = 0;
	if (*word_type == W_SPACE || *word_type == W_SARED)
	{
		start = *i;
		while (*word_type == W_SPACE || *word_type == W_SARED)
		{
			(*i)++;
			*word_type = matrix[*word_type][get_token_type(str[*i])];
			result = ft_substr(str, start, *i - start);
			printf("------------------(extarct space or sared)---------------------\n");
			checkposition(*word_type, str, *i);
			checkinput(get_token_type(str[*i]));
		}
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

char	*extract_redirect(t_word_type *word_type, int *i, char *str, const int matrix[W_TOTAL][NUM_INPUT], t_word_type *previous_word_type)
{
	int		start;
	char	*result;

	start = 0;
	if (*word_type == W_REDIN || *word_type == W_REDOU)
	{
		*previous_word_type = *word_type;
		start = *i;
		(*i)++;
		*word_type = matrix[*word_type][get_token_type(str[*i])];
		printf("---------------(extract redirect)------------------------\n");
		checkposition(*word_type, str, *i);
		checkinput(get_token_type(str[*i]));
		if(*word_type == W_REDAP || *word_type == W_HRDOC)
		{
			(*i)++;
			*word_type = matrix[*word_type][get_token_type(str[*i])];
			printf("---------------(extract append o hrdoc)------------------------\n");
			checkposition(*word_type, str, *i);
			checkinput(get_token_type(str[*i]));
		}
		result = ft_substr(str, start, *i - start);
		printf("value of extracted result when redirect is %s\n", result);
		printf("---------------------------------------------------------------\n");
		return (result);
	}
	else
	return (NULL);
}

char	*extract_word(t_word_type *word_type, int *i, char *str, const int matrix[W_TOTAL][NUM_INPUT], t_word_type *previous_word_type)
{
	int		start;
	char	*result;
	
	start = *i;
	if ((*word_type == W_DOUBQ || *word_type == W_SINGQ) && (*previous_word_type == W_EOSTD || *previous_word_type == W_EOSTS))
		start = *i - 1;
	while (*word_type == W_DOUBQ || *word_type == W_SINGQ || *word_type == W_STNDR)
	{
		*previous_word_type = *word_type;
		(*i)++;
		*word_type = matrix[*word_type][get_token_type(str[*i])];
		printf("---------------(extract_word)------------------------\n");
		checkposition(*word_type, str, *i);
		checkinput(get_token_type(str[*i]));
	}
	if (*word_type == W_EOFDQ || *word_type == W_EOFSQ)
		result = ft_substr(str, start + 1, *i - start - 1);
	else
		result = ft_substr(str, start, *i - start);
	if (*word_type == W_EOFDQ || *word_type == W_EOFSQ)
	{
		(*i)++;
		*word_type = matrix[*word_type][get_token_type(str[*i])];
	}
	if (!result)
	{
		printf("Error\n");
		return (NULL);
	}
	return (result);
}

char	*extract_token_value(char *str, int *i, const int matrix[W_TOTAL][NUM_INPUT], t_word_type *word_type, t_word_type *previous_word_type)
{
	char	*result;
	
	result = extract_space_or_sared(word_type, i, str, matrix);
	if (result)
	return (result);
		result = extract_redirect(word_type, i, str, matrix, previous_word_type);
		if (result)
		return (result);
	result = extract_word(word_type, i, str, matrix, previous_word_type);
	if (result)
	return (result);
	else
	return (NULL);
}


	

	





