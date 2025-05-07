/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernando <fernando@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:22:28 by fdurban-          #+#    #+#             */
/*   Updated: 2025/05/06 16:40:21 by fernando         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
#define TOKENIZER_H

// "state"
typedef enum e_word_type
{
	WORD_START,
	WORD_STANDARD,
	WORD_SINGLE_QUOTE,
	WORD_DOUBLE_QUOTE,
	WORD_REDIRECT_IN,
	WORD_REDIRECT_OUT,
	WORD_REDIRECT_APPEND,
	WORD_HEREDOC,
	WORD_SPACE,
	WORD_SPACE_AFTER_REDIRECT,
	WORD_END_OF_SINGLE_QUOTE,
	WORD_END_OF_DOUBLE_QUOTE,
	WORD_END_STANDARD,
	WORD_END_STANDARD_TO_DOUBLE_QUOTE,
	WORD_END_STANDARD_TO_SINGLE_QUOTE,
	WORD_END,
	WORD_ERROR,
	NUM_WORDS
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

char**	tokenize(char *valid_input);

#endif