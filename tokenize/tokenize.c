/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernando <fernando@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:55:33 by fdurban-          #+#    #+#             */
/*   Updated: 2025/04/18 12:39:45 by fernando         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	tokenize(char *valid_input, int num_tokens)
{
	char	**tokens;
	tokens = ft_split(valid_input, '|');
	
}