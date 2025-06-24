/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:25:55 by fdurban-          #+#    #+#             */
/*   Updated: 2025/06/24 18:10:42 by fdurban-         ###   ########.fr       */
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
		handle_token_expansion(ctx->previous_word_type, &ctx->command_node,
			shell);
		handle_token_join(ctx);
	}
}

void	free_command_part_list(t_command_part *lst)
{
	t_command_part	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst->value);
		free(lst);
		lst = tmp;
	}
}

static void	cleanup_tokenizer_ctx(t_tokenizer_ctx *ctx)
{
	if (ctx->lst)
		free_command_part_list(ctx->lst);
	if (ctx->partial_token)
		free(ctx->partial_token);
	if (ctx->command_token)
		free(ctx->command_token);
	if (ctx->command_node)
		free(ctx->command_node);
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
			printf("Syntax Error!\n");
			cleanup_tokenizer_ctx(&ctx);
			ctx.lst = NULL;
			break ;
		}
		process_token(matrix, valid_command, shell, &ctx);
		if (ctx.word_type == W_ERROR)
		{
			printf("Syntax Error!\n");
			cleanup_tokenizer_ctx(&ctx);
			ctx.lst = NULL;
			break ;
		}
	}
	return (ctx.lst);
}

int	count_segments(const char *cmd, const int matrix[W_TOTAL][I_NUM_INPUT])
{
	int	i;
	int	state;
	int	count;
	int	input;

	state = W_START;
	count = 1;
	i = 0;
	while (cmd[i])
	{
		input = get_token_type(cmd[i]);
		state = matrix[state][input];
		i++;
		if (state == W___END)
			count++;
	}
	return (count);
}

void	fill_segments(char **segments, char *cmd,
		const int matrix[W_TOTAL][I_NUM_INPUT])
{
	int	i;
	int	state;
	int	start;
	int	segment_index;
	int	input;

	i = 0;
	state = W_START;
	start = 0;
	segment_index = 0;
	while (cmd[i])
	{
		input = get_token_type(cmd[i]);
		state = matrix[state][input];

		i++;
		if (state == W___END || cmd[i] == '\0')
		{
			segments[segment_index++] = ft_substr(cmd, start, i - start);
			start = i;
			state = W_START;
		}
	}
	segments[segment_index] = NULL;
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


int	validate_command_syntax(char *valid_command, const int matrix[W_TOTAL][I_NUM_INPUT])
{
	int	i = 0;
	int	state = W_START;
	int	input;

	while (valid_command[i])
	{
		input = get_token_type(valid_command[i]);

		if (valid_command[i] == '|')
		{
			int j = i + 1;
			while (valid_command[j] && valid_command[j] == ' ')
				j++;
			if (valid_command[j] == '\0')
			{
				printf("Syntax error: unexpected end after pipe\n");
				return (1);
			}
		}
		state = matrix[state][input];
		//checkposition(state, valid_command, i);
		if (state == W_ERROR)
		{
			printf("Syntax Error\n");
			return (1);
		}
		i++;
	}
	return (0);
}

t_command_part	**tokenize(char *valid_command, t_shell *shell)
{
	t_command_part	**token;
	//space // letter // end // singq // douq     // redin  //redou // pipe
	const int		matrix[W_TOTAL][I_NUM_INPUT] = {
	{W_START, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU, W_ERROR}, //start
	{W_SPACE, W_STNDR, W___END, W_EOSTS, W_EOSTD, W_REDIN, W_REDOU, W___END}, //stndr
	{W_SINGQ, W_SINGQ, W_ERROR, W_EOFSQ, W_SINGQ, W_SINGQ, W_SINGQ, W_SINGQ}, //singq
	{W_DOUBQ, W_DOUBQ, W_ERROR, W_DOUBQ, W_EOFDQ, W_DOUBQ, W_DOUBQ, W_DOUBQ}, //doubq
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_HRDOC, W_ERROR, W_ERROR}, //redin
	{W_SARED, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDAP, W_ERROR}, //redou
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR, W_ERROR}, //redap
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR, W_ERROR}, //hrdoc
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU, W___END}, //space
	{W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR, W_ERROR}, //sared
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU, W___END}, //EOFSQ
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU, W___END}, //EOFDQ
	{W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU, W___END}, //EOFST
	{W_DOUBQ, W_DOUBQ, W___END, W_EOFDQ, W_EOFDQ, W_DOUBQ, W_DOUBQ, W_ERROR}, //EOSTD
	{W_SINGQ, W_SINGQ, W___END, W_EOFSQ, W_SINGQ, W_SINGQ, W_SINGQ, W_ERROR}, //EOSTS
	};
	if(validate_command_syntax(valid_command, matrix))
		return (NULL);
	token = split_and_tokenize(matrix, valid_command, shell);
	//print_values(token);
	return (token);
}
