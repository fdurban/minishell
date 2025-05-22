/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernando <fernando@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:22:28 by fdurban-          #+#    #+#             */
/*   Updated: 2025/05/22 20:42:35 by fernando         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
#define TOKENIZER_H
# include "env.h"
# include "minishell.h"

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

t_command_part	**tokenize(char *valid_command, t_env *env);
char	*extract_token_value(char *str, int *i, const int matrix[W_TOTAL][NUM_INPUT], t_word_type *word_type, t_word_type *previous_word_type);
char	*expand_token(t_command_part *word, t_env *env);
int	get_token_type(char c);
void			checkposition(t_word_type word_type, char *valid_command, int i);
void	checkinput(t_input_tokenizer input);
void			print_values(t_command_part **results);

#endif

// space //letter // end // single quote //double quote //redirect IN // redirect out
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