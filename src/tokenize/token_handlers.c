/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:37:15 by fernando          #+#    #+#             */
/*   Updated: 2025/07/19 13:58:05 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	needs_retokenize(t_tokenizer_ctx *ctx,
char *original, char *expanded)
{
	if (!expanded || !*expanded || ctx->is_assign)
		return (0);
	if (ft_strcmp(original, expanded) == 0)
		return (0);
	if (ft_strcmp(expanded, ">>") == 0 || ft_strcmp(expanded, ">") == 0
		|| ft_strcmp(expanded, "<<") == 0
		|| ft_strcmp(expanded, "<") == 0)
		return (0);
	if (ctx->previous_word_type != W_DOUBQ
		&& (ft_strchr(expanded, ' ') || ft_strchr(expanded, '<')
			|| ft_strchr(expanded, '>') || ft_strchr(expanded, '|')))
		return (1);
	return (0);
}

void	handle_token_expansion(t_tokenizer_ctx *ctx, t_shell *shell)
{
	char	*expanded;
	char	*original;

	if ((ctx->previous_word_type == W_STNDR
			|| ctx->previous_word_type == W_DOUBQ)
		&& !ctx->here_doc
		&& ft_strchr(ctx->command_node->value, '$'))
	{
		original = ft_strdup(ctx->command_node->value);
		if (!original)
			return ;
		expanded = expand_token(ctx->command_node->value, shell);
		free(ctx->command_node->value);
		ctx->command_node->value = expanded;
		ctx->command_node->needs_retokenize = needs_retokenize(ctx,
				original, expanded);
		ctx->is_assign = 0;
		free(original);
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

static void	create_node_from_partial(t_tokenizer_ctx *ctx)
{
	t_command_part	*joined_node;

	joined_node = create_command_node(ctx->partial_token,
			ctx->previous_word_type);
	joined_node->needs_retokenize = ctx->command_node->needs_retokenize;
	ctx->command_node->needs_retokenize = 0;
	add_command_part_to_list(&ctx->lst, joined_node);
	free(ctx->partial_token);
	ctx->partial_token = NULL;
}

void	handle_token_join(t_tokenizer_ctx *ctx)
{
	if (should_accumulate_token(ctx->previous_word_type))
		accumulate_partial_token(ctx);
	if (should_create_node_from_partial(ctx->word_type, ctx->partial_token))
		create_node_from_partial(ctx);
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
