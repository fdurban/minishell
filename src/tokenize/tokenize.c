/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:25:55 by fdurban-          #+#    #+#             */
/*   Updated: 2025/07/19 12:13:42 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_word_type	get_next_word_type(const int matrix[W_TOTAL][I_NUM_INPUT],
		char *str, int *i, t_word_type current)
{
	t_input_tokenizer	input;

	if (current != W_REDOU && current != W_REDIN)
	{
		input = get_token_type(str[*i]);
		current = matrix[current][input];
	}
	if (current == W_START)
	{
		(*i)++;
		input = get_token_type(str[*i]);
		current = matrix[current][input];
	}
	return (current);
}

static void	process_token(const int matrix[W_TOTAL][I_NUM_INPUT],
		char *valid_command, t_shell *shell, t_tokenizer_ctx *ctx)
{
	ctx->command_token = NULL;
	if (ctx->word_type == W_SINGQ || ctx->word_type == W_DOUBQ
		|| ctx->word_type == W_STNDR || ctx->word_type == W_SARED
		|| ctx->word_type == W_SPACE || ctx->word_type == W_REDIN
		|| ctx->word_type == W_REDOU)
	{
		ctx->command_token = extract_token_value(valid_command, matrix, ctx);
	}
	if (ctx->command_token)
	{
		ctx->command_node = create_command_node(ctx->command_token,
				ctx->previous_word_type);
		free(ctx->command_token);
		ctx->command_token = NULL;
		handle_token_expansion(ctx, shell);
		handle_token_join(ctx);
	}
}

t_command_part	*tokenize_pipe_segment(const int matrix[W_TOTAL][I_NUM_INPUT],
		char *valid_command, t_shell *shell)
{
	t_tokenizer_ctx	ctx;

	ctx.i = 0;
	ctx.word_type = W_START;
	ctx.command_token = NULL;
	ctx.command_node = NULL;
	ctx.partial_token = NULL;
	ctx.lst = NULL;
	ctx.here_doc = 0;
	ctx.is_assign = 0;
	while (ctx.word_type != W___END && ctx.word_type != W_EPIPE)
	{
		ctx.previous_word_type = ctx.word_type;
		ctx.word_type = get_next_word_type(matrix, valid_command, &ctx.i,
				ctx.word_type);
		process_token(matrix, valid_command, shell, &ctx);
	}
	return (ctx.lst);
}

t_command_part	**split_and_tokenize(const int matrix[W_TOTAL][I_NUM_INPUT],
		char *valid_command, t_shell *shell)
{
	char			**segments;
	int				count;
	t_command_part	**results;
	int				i;

	count = count_segments(valid_command, matrix);
	segments = malloc(sizeof(char *) * (count + 1));
	fill_segments(segments, valid_command, matrix);
	i = 0;
	results = malloc(sizeof(t_command_part *) * (count + 1));
	while (segments[i])
	{
		results[i] = tokenize_pipe_segment(matrix, segments[i], shell);
		i++;
	}
	results[i] = NULL;
	i = 0;
	while (segments[i])
		free(segments[i++]);
	free(segments);
	return (results);
}

t_command_part	**tokenize(char *valid_command, t_shell *shell)
{
	t_command_part	**token;
	const int		matrix[W_TOTAL][I_NUM_INPUT] = {
	{W_START, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU, W_ERROR},
	{W_SPACE, W_STNDR, W___END, W_EOSTS, W_EOSTD, W_REDIN, W_REDOU, W_EPIPE},
	{W_SINGQ, W_SINGQ, W_ERROR, W_EOFSQ, W_SINGQ, W_SINGQ, W_SINGQ, W_SINGQ},
	{W_DOUBQ, W_DOUBQ, W_ERROR, W_DOUBQ, W_EOFDQ, W_DOUBQ, W_DOUBQ, W_DOUBQ},
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_HRDOC, W_ERROR, W_ERROR},
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_REDAP, W_ERROR},
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR, W_ERROR},
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR, W_ERROR},
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU, W_EPIPE},
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR, W_ERROR},
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU, W_EPIPE},
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU, W_EPIPE},
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU, W_EPIPE},
	{W_DOUBQ, W_DOUBQ, W_ERROR, W_EOFDQ, W_EOFDQ, W_DOUBQ, W_DOUBQ, W_ERROR},
	{W_SINGQ, W_SINGQ, W_ERROR, W_EOFSQ, W_SINGQ, W_SINGQ, W_SINGQ, W_ERROR},
	{W_START, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR, W_ERROR}
	};
	if (validate_command_syntax(valid_command, matrix))
		return (NULL);
	token = split_and_tokenize(matrix, valid_command, shell);
	retokenize(token, shell);
	return (token);
}
