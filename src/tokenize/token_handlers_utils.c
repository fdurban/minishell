/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:49:23 by fdurban-          #+#    #+#             */
/*   Updated: 2025/07/15 17:21:00 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_word_type(const char *str, t_tokenizer_ctx *ctx,
	const int matrix[W_TOTAL][I_NUM_INPUT])
{
	int	input;

	input = get_token_type(str[ctx->i]);
	ctx->word_type = matrix[ctx->word_type][input];
}

int	should_accumulate_token(int prev_type)
{
	return (prev_type == W_STNDR || prev_type == W_DOUBQ
		|| prev_type == W_SINGQ);
}

int	should_create_node_from_partial(int type, char *partial)
{
	return ((type == W_SPACE || type == W_SARED || type == W_REDIN
			|| type == W_REDOU || type == W___END) && partial);
}

int	should_add_command_node(int prev_type, int type, char *partial)
{
	return ((prev_type == W_REDAP || prev_type == W_HRDOC
			|| prev_type == W_REDIN || prev_type == W_REDOU
			|| type == W___END) && !partial);
}

static int	calc_start(t_tokenizer_ctx *ctx)
{
	if ((ctx->word_type == W_DOUBQ || ctx->word_type == W_SINGQ)
		&& (ctx->previous_word_type == W_EOSTD
			|| ctx->previous_word_type == W_EOSTS))
		return (ctx->i - 1);
	return (ctx->i);
}
