/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:30:02 by fdurban-          #+#    #+#             */
/*   Updated: 2025/06/13 18:12:36 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_token_type(char c)
{
	if (c == ' ')
		return (I_SPACE);
	else if (c == '\'')
		return (I_IN_SINGLE_QUOTE);
	else if (c == '<')
		return (I_REDIRECT_IN);
	else if (c == '>')
		return (I_REDIRECT_OUT);
	else if (c == '"')
		return (I_IN_DOUBLE_QUOTE);
	else if (c == '\0')
		return (I_END);
	else if (c == '|')
		return (I_PIPE);
	else if (c >= 33 && c <= 126)
		return (I_LETTER);
	else
		return (0);
}

void	skip_space_or_sared(char *str, const int matrix[W_TOTAL][I_NUM_INPUT], t_tokenizer_ctx *ctx)
{
	while (ctx->word_type == W_SPACE || ctx->word_type == W_SARED)
	{
		ctx->i++;
		ctx->word_type = matrix[ctx->word_type][get_token_type(str[ctx->i])];
	}
}

char	*extract_redirect(char *str, const int matrix[W_TOTAL][I_NUM_INPUT], t_tokenizer_ctx *ctx)
{
	int		start;
	char	*result;

	start = 0;
	if (ctx->word_type == W_REDIN || ctx->word_type == W_REDOU)
	{
		ctx->previous_word_type = ctx->word_type;
		start = ctx->i;
		ctx->i++;
		ctx->word_type = matrix[ctx->word_type][get_token_type(str[ctx->i])];
		if (ctx->word_type == W_REDAP || ctx->word_type == W_HRDOC)
		{
			ctx->previous_word_type = ctx->word_type;
			ctx->i++;
			ctx->word_type = matrix[ctx->word_type][get_token_type(str[ctx->i])];
		}
		result = ft_substr(str, start, ctx->i - start);
		return (result);
	}
	else
		return (NULL);
	}

char	*extract_word(char *str, const int matrix[W_TOTAL][I_NUM_INPUT], t_tokenizer_ctx *ctx)
{
	int		start;
	char	*result;
	
	start = ctx->i;
	if ((ctx->word_type == W_DOUBQ || ctx->word_type == W_SINGQ) && (ctx->previous_word_type == W_EOSTD || ctx->previous_word_type == W_EOSTS))
		start = ctx->i - 1;
	while (ctx->word_type == W_DOUBQ || ctx->word_type == W_SINGQ || ctx->word_type == W_STNDR)
	{
		ctx->previous_word_type = ctx->word_type;
	ctx->i++;
	ctx->word_type = matrix[ctx->word_type][get_token_type(str[ctx->i])];
	}
	if (ctx->word_type == W_EOFDQ || ctx->word_type == W_EOFSQ)
		result = ft_substr(str, start + 1, ctx->i - start - 1);
		else
		result = ft_substr(str, start, ctx->i - start);
		if (ctx->word_type == W_EOFDQ || ctx->word_type == W_EOFSQ)
		{
			ctx->i++;
		ctx->word_type = matrix[ctx->word_type][get_token_type(str[ctx->i])];
	}
	if (!result)
		printf("Error\n");
	return (result);
}

char	*extract_token_value(char *str, const int matrix[W_TOTAL][I_NUM_INPUT], t_tokenizer_ctx *ctx)
{
	char	*result;
	
	result = NULL;
	skip_space_or_sared(str, matrix, ctx);
	result = extract_redirect(str, matrix, ctx);
	if (result)
	return (result);
	result = extract_word(str, matrix, ctx);
	if (result)
	return (result);
	else
	return (NULL);
}

// printf("---------------(extract append o hrdoc)------------------------\n");
// checkposition(*word_type, str, *i);
// checkinput(get_token_type(str[*i]));

// printf("value of extracted result when redirect is %s\n", result);
// printf("---------------------------------------------------------------\n");

// printf("---------------(extract_word)------------------------\n");
// checkposition(ctx->word_type, str, ctx->i);
// checkinput(get_token_type(str[ctx->i]));
// printf("---------------(extract redirect)------------------------\n");
// checkposition(ctx->word_type, str, ctx->i);
// checkinput(get_token_type(str[ctx->i]));
// printf("-------------(skip space or sared)----------------\n");
// checkposition(ctx->word_type, str, ctx->i);
// checkinput(get_token_type(str[ctx->i]));