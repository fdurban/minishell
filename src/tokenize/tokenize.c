/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/06/12 12:41:56 by igngonza         ###   ########.fr       */
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
		ctx->command_token = extract_token_value(valid_command, matrix, ctx);
	if (ctx->command_token)
	{
		ctx->command_node = create_command_node(ctx->command_token,
				ctx->previous_word_type);
		handle_token_expansion(ctx->previous_word_type, &ctx->command_node,
			shell);
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
	while (ctx.word_type != W___END)
	{
		ctx.previous_word_type = ctx.word_type;
		ctx.word_type = get_next_word_type(matrix, valid_command, &ctx.i,
				ctx.word_type);
		if (ctx.word_type == W_ERROR)
		{
			printf("Syntax error!\n");
			break ;
		}
		process_token(matrix, valid_command, shell, &ctx);
		if (ctx.word_type == W_ERROR)
		{
			printf("Syntax Error!\n");
			break ;
		}
	}
	return (ctx.lst);
}

t_command_part	**split_and_tokenize(const int matrix[W_TOTAL][I_NUM_INPUT],
		char *valid_command, t_shell *shell)
{
	char			**tokens;
	int				count;
	int				i;
	t_command_part	**results;

	count = 0;
	i = 0;
	tokens = ft_split(valid_command, '|');
	if (!tokens)
		return (NULL);
	while (tokens[count])
		count++;
	results = malloc(sizeof(t_command_part *) * (count + 1));
	while (tokens[i])
	{
		results[i] = tokenize_pipe_segment(matrix, tokens[i], shell);
		i++;
	}
	results[i] = NULL;
	return (results);
}

t_command_part	**tokenize(char *valid_command, t_shell *shell)
{
	t_command_part	**token;

	const int matrix[W_TOTAL][I_NUM_INPUT] = {
		{W_START, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU},
		// W_START
		{W_SPACE, W_STNDR, W___END, W_EOSTS, W_EOSTD, W_REDIN, W_REDOU},
		// W_STNDR
		{W_SINGQ, W_SINGQ, W_ERROR, W_EOFSQ, W_SINGQ, W_SINGQ, W_SINGQ},
		// WORD_SINGLE QUOTE
		{W_DOUBQ, W_DOUBQ, W_ERROR, W_DOUBQ, W_EOFDQ, W_DOUBQ, W_DOUBQ},
		// WORD_DOUBLE QUOTE
		{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_HRDOC, W_ERROR},
		// REDIRECT_IN
		{W_SARED, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDAP},
		// REDIRECT_OUT
		{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR},
		// REDIRECT_APPEND
		{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR},
		// HERE_DOC
		{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU},
		// SPACE AFTER WORD
		{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR},
		// SPACE AFTER REDIRECT
		{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU},
		// END OF SINGLE QUOTE
		{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU},
		// END OF DOUBLE QUOTE
		{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU},
		// END OF STANDARD
		{W_DOUBQ, W_DOUBQ, W___END, W_EOFDQ, W_EOFDQ, W_DOUBQ, W_DOUBQ},
		// END OF STANDARD TO DOUBLE QUOTE
		{W_SINGQ, W_SINGQ, W___END, W_EOFSQ, W_SINGQ, W_SINGQ, W_SINGQ}
		// END OF STANDARD TO SINGLE QUOTE
	};
	token = split_and_tokenize(matrix, valid_command, shell);
	// print_values(token);
	return (token);
}

// space //letter // end // single quote //double quote //redirect IN
// redirect out
// space //letter // end // single quote //double quote //redirect IN
// redirect out