/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:55:33 by fdurban-          #+#    #+#             */
/*   Updated: 2025/04/22 17:37:53 by fdurban-         ###   ########.fr       */
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
char	**tokenize(char *valid_input, int token_number)
{
	int				*i;
	char			**tokens;
	//t_command_part	**command_list;
	char			***parsed_tokens;
	//char			*token;

	tokens = ft_split(valid_input, '|');
	i = 0;
	parsed_tokens =  malloc(sizeof(char **) * token_number + 1);
	//sustituir por funciones que partan las diferentes partes del pipe y lo ponga en una lista enlazada
	return (tokens);
}