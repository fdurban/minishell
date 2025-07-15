/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:30:02 by fdurban-          #+#    #+#             */
/*   Updated: 2025/07/15 17:20:53 by fdurban-         ###   ########.fr       */
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

void	skip_space_or_sared(char *str,
const int matrix[W_TOTAL][I_NUM_INPUT], t_tokenizer_ctx *ctx)
{
	while (ctx->word_type == W_SPACE || ctx->word_type == W_SARED)
	{
		ctx->i++;
		update_word_type(str, ctx, matrix);
	}
}

char	*extract_redirect(char *str,
const int matrix[W_TOTAL][I_NUM_INPUT], t_tokenizer_ctx *ctx)
{
	int		start;
	char	*result;

	start = 0;
	if (ctx->word_type == W_REDIN || ctx->word_type == W_REDOU)
	{
		ctx->previous_word_type = ctx->word_type;
		start = ctx->i;
		ctx->i++;
		update_word_type(str, ctx, matrix);
		if (ctx->word_type == W_REDAP || ctx->word_type == W_HRDOC)
		{
			ctx->here_doc = 1;
			ctx->previous_word_type = ctx->word_type;
			ctx->i++;
			update_word_type(str, ctx, matrix);
		}
		result = ft_substr(str, start, ctx->i - start);
		return (result);
	}
	else
		return (NULL);
}

char	*extract_word(char *str,
const int matrix[W_TOTAL][I_NUM_INPUT], t_tokenizer_ctx *ctx)
{
	int		start;
	char	*result;

	start = calc_start(ctx);
	while (ctx->word_type == W_DOUBQ
		|| ctx->word_type == W_SINGQ || ctx->word_type == W_STNDR)
	{
		if (ctx->word_type == W_STNDR && str[ctx->i] == '=')
			ctx->is_assign = 1;
		ctx->previous_word_type = ctx->word_type;
		ctx->i++;
		update_word_type(str, ctx, matrix);
	}
	if (ctx->word_type == W_EOFDQ || ctx->word_type == W_EOFSQ)
		result = ft_substr(str, start + 1, ctx->i - start - 1);
	else
		result = ft_substr(str, start, ctx->i - start);
	if (ctx->word_type == W_EOFDQ || ctx->word_type == W_EOFSQ)
	{
		ctx->i++;
		update_word_type(str, ctx, matrix);
	}
	return (result);
}

char	*extract_token_value(char *str,
const int matrix[W_TOTAL][I_NUM_INPUT], t_tokenizer_ctx *ctx)
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
