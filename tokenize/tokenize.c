/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:55:33 by fdurban-          #+#    #+#             */
/*   Updated: 2025/04/21 16:45:10 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_tokens_by_space(char ***parsed_tokens, int token_number)
{
	int i = 0;
	while (i < token_number)
	{
		int j = 0;
		while(parsed_tokens[i][j])
		{
			printf("[%s]\n", parsed_tokens[i][j]);
			j++;
		}
		i++;
	}
}

char	*extract_word(char *str, int *i)
{
	int start = *i;
	while (str)
	{
		
	}
}

char	**tokenize(char *valid_input, int token_number)
{
	int		i;
	char	**tokens;
	tokens = ft_split(valid_input, '|');
	i = 0;
	char	***parsed_tokens;	

	parsed_tokens =  malloc(sizeof(char **) * token_number + 1);
	printf("This is the number of tokens %d\n", token_number);
	//sustituir por funciones que partan las diferentes partes del pipe y lo ponga en una lista enlazada
	while(i < token_number)
	{
		parsed_tokens[i] = ft_split(tokens[i], ' ');
		i++;
	}
	print_tokens_by_space(parsed_tokens, token_number);
	// i = 0;
	// while (tokens[i])
	// {
	// 	printf("%s\n", tokens[i]);
	// 	i++;
	// }
	return (tokens);
}
