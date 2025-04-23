/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:22:28 by fdurban-          #+#    #+#             */
/*   Updated: 2025/04/23 14:07:10 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
#define TOKENIZER_H

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
	WORD_END_OF_SINGLE_QUOTE,
	WORD_END_OF_DOUBLE_QUOTE,
	WORD_END,
	WORD_ERROR,
	NUM_WORDS
} t_word_type;

typedef enum e_input
{
	INPUT_PIPE,
	INPUT_SPACE,
	INPUT_LETTER,
	INPUT_END,
	INPUT_IN_SINGLE_QUOTE,
	INPUT_IN_DOUBLE_QUOTE,
	INPUT_REDIRECT,
	NUM_INPUT
}	t_input;

#endif