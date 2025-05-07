/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:22:28 by fdurban-          #+#    #+#             */
/*   Updated: 2025/05/07 16:04:50 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
#define TOKENIZER_H

// "state"
typedef enum e_word_type
{
	W_START,
	W_STNDR,
	W_SINGQ,
	W_DOUBQ,
	W_REDIN,
	W_REDOU,
	W_REDAP,
	W_HRDOC,
	W_SPACE,
	W_SARED,
	W_EOFSQ,
	W_EOFDQ,
	W_EOFST,
	W_EOSTD,
	W_EOSTS,
	W___END,
	W_ERROR,
	W_TOTAL
} t_word_type;

typedef enum e_input_tokenizer
{
	TOKEN_SPACE,
	TOKEN_LETTER,
	TOKEN_END,
	TOKEN_IN_SINGLE_QUOTE,
	TOKEN_IN_DOUBLE_QUOTE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_NUM_INPUT
}	t_input_tokenizer;

typedef struct s_command_part
{
	char	*value;
	t_word_type	type;
	struct s_command_part	*next;
} t_command_part;

	// const int	matrix[W_TOTAL][NUM_INPUT] = {
	// {W_START, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, //W_START
	// {W_EOFST, W_STNDR, W___END, W_EOSTS, W_EOSTD, W_EOFST, W_EOFST}, // W_STNDR
	// {W_SINGQ, W_SINGQ, W_ERROR, W_EOFSQ, W_SINGQ, W_SINGQ, W_SINGQ}, // WORD_SINGLE QUOTE
	// {W_DOUBQ, W_DOUBQ, W_STNDR, W_DOUBQ, W_EOFDQ, W_DOUBQ, W_DOUBQ}, // WORD_DOUBLE QUOTE
	// {W_SPACE, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_HRDOC, W_ERROR}, // REDIRECT_IN
	// {W_REDOU, W_REDOU, W___END, W_REDOU, W_REDOU, W_REDIN, W_REDAP}, // REDIRECT_OUT
	// {W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR}, // REDIRECT_APPEND
	// {W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR}, // HERE_DOC
	// {W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, // SPACE AFTER WORD
	// {W_SARED, W_STNDR, W_ERROR, W_SINGQ, W_DOUBQ, W_ERROR, W_ERROR}, // SPACE AFTER REDIRECT
	// {W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, // END OF SINGLE QUOTE
	// {W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, // END OF DOUBLE QUOTE
	// {W_SPACE, W_STNDR, W___END, W_SINGQ, W_DOUBQ, W_REDIN, W_REDOU}, // END OF STANDARD
	// {W_DOUBQ, W_DOUBQ, W___END, W_EOFDQ, W_EOFDQ, W_DOUBQ, W_DOUBQ}, // END OF STANDARD TO DOUBLE QUOTE
	// {W_SINGQ, W_SINGQ, W___END, W_EOFSQ, W_SINGQ, W_SINGQ, W_SINGQ} // END OF STANDARD TO DOUBLE QUOTE
	// };

char**	tokenize(char *valid_input);
void 	get_word_transition_matrix(int matrix[17][7]);
#endif


/*
	WORD_START ->						W_START
	WORD_STANDARD->						W_STNDR
	WORD_SINGLE_QUOTE ->				W_SINGQ
	WORD_DOUBLE_QUOTE ->				W_DOUBQ
	WORD_REDIRECT_IN ->					W_REDIN
	WORD_REDIRECT_OUT ->				W_REDOU
	WORD_REDIRECT_APPEND ->				W_REDAP
	WORD_HEREDOC ->						W_HRDOC
	WORD_SPACE ->						W_SPACE
	WORD_SPACE_AFTER_REDIRECT			W_SARED
	WORD_END_OF_SINGLE_QUOTE			W_EOFSQ
	WORD_END_OF_DOUBLE_QUOTE			W_EOFDQ
	WORD_END_STANDARD					W_EOFST
	WORD_END_STANDARD_TO_DOUBLE_QUOTE	W_EOSTD
	WORD_END_STANDARD_TO_SINGLE_QUOTE,	W_EOSTS
	WORD_END							W___END
	WORD_ERROR							W_ERROR
	NUM_WORDS							W_TOTAL
*/