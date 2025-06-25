/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:37:15 by fernando          #+#    #+#             */
/*   Updated: 2025/06/25 15:49:55 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_token_expansion(t_word_type previous_word_type,
		t_command_part **command_node, t_shell *shell, t_tokenizer_ctx *ctx)
{
	char	*expanded;

	if ((previous_word_type == W_STNDR
			|| previous_word_type == W_DOUBQ) && !ctx->here_doc)
	{
		expanded = expand_token((*command_node)->value, shell);
		free((*command_node)->value);
		(*command_node)->value = expanded;
	}
}

static void	accumulate_partial_token(t_tokenizer_ctx *ctx)
{
	char	*joined;

	joined = NULL;
	if (ctx->partial_token == NULL)
		ctx->partial_token = ft_strdup(ctx->command_node->value);
	else
	{
		joined = ft_strjoin(ctx->partial_token, ctx->command_node->value);
		free(ctx->partial_token);
		ctx->partial_token = joined;
	}
}

void	handle_token_join(t_tokenizer_ctx *ctx)
{
	t_command_part	*joined_node;

	joined_node = NULL;
	if (should_accumulate_token(ctx->previous_word_type))
		accumulate_partial_token(ctx);
	if (should_create_node_from_partial(ctx->word_type, ctx->partial_token))
	{
		joined_node = create_command_node(ctx->partial_token,
				ctx->previous_word_type);
		add_command_part_to_list(&ctx->lst, joined_node);
		free(ctx->partial_token);
		ctx->partial_token = NULL;
	}
	else if (should_add_command_node(ctx->previous_word_type,
			ctx->word_type, ctx->partial_token))
	{
		add_command_part_to_list(&ctx->lst, ctx->command_node);
		ctx->command_node = NULL;
	}
	if (ctx->command_node)
	{
		free(ctx->command_node->value);
		free(ctx->command_node);
		ctx->command_node = NULL;
	}
}
