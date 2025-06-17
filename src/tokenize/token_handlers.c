/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:37:15 by fernando          #+#    #+#             */
/*   Updated: 2025/06/17 14:54:27 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_token_expansion(t_word_type previous_word_type,
		t_command_part **command_node, t_shell *shell)
{
	char	*expanded;

	if (previous_word_type == W_STNDR || previous_word_type == W_DOUBQ)
	{
		expanded = expand_token(*command_node, shell);
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
	if (ctx->previous_word_type == W_STNDR || ctx->previous_word_type == W_DOUBQ
		|| ctx->previous_word_type == W_SINGQ)
		accumulate_partial_token(ctx);
	if ((ctx->word_type == W_SPACE || ctx->word_type == W_SARED
			|| ctx->word_type == W_REDIN || ctx->word_type == W_REDOU
			|| ctx->word_type == W___END) && ctx->partial_token)
	{
		joined_node = create_command_node(ctx->partial_token,
				ctx->previous_word_type);
		add_command_part_to_list(&ctx->lst, joined_node);
		free(ctx->partial_token);
		ctx->partial_token = NULL;
	}
	else if ((ctx->previous_word_type == W_REDAP
			|| ctx->previous_word_type == W_HRDOC
			|| ctx->previous_word_type == W_REDIN
			|| ctx->previous_word_type == W_REDOU || ctx->word_type == W___END)
		&& !ctx->partial_token)
	{
		if (ctx->command_node && ctx->command_node->value[0] == '\0')
		// Handle empty tokens
		{
			add_command_part_to_list(&ctx->lst, ctx->command_node);
			ctx->command_node = NULL;
		}
		else
		{
			add_command_part_to_list(&ctx->lst, ctx->command_node);
			ctx->command_node = NULL;
		}
	}
}
