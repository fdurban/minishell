/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:22:28 by fdurban-          #+#    #+#             */
/*   Updated: 2025/06/13 18:04:00 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H
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
}							t_word_type;

typedef enum e_input_tokenizer
{
	I_SPACE,
	I_LETTER,
	I_END,
	I_IN_SINGLE_QUOTE,
	I_IN_DOUBLE_QUOTE,
	I_REDIRECT_IN,
	I_REDIRECT_OUT,
	I_PIPE,
	I_NUM_INPUT
}							t_input_tokenizer;

typedef struct s_command_part
{
	char					*value;
	t_word_type				type;
	struct s_command_part	*next;
}							t_command_part;

typedef struct s_tokenizer_ctx
{
	int						i;
	t_word_type				word_type;
	t_word_type				previous_word_type;
	char					*command_token;
	t_command_part			*command_node;
	char					*partial_token;
	t_command_part			*lst;
}							t_tokenizer_ctx;

t_command_part				**tokenize(char *valid_command, t_shell *shell);
char						*extract_token_value(char *str,
								const int matrix[W_TOTAL][I_NUM_INPUT],
								t_tokenizer_ctx *ctx);
char						*expand_token(t_command_part *word, t_shell *shell);
int							get_token_type(char c);
void						checkposition(t_word_type word_type,
								char *valid_command, int i);
void						checkinput(t_input_tokenizer input);
void						print_values(t_command_part **results);
char						**tokens_to_argv(t_command_part *lst);
t_command_part				*create_command_node(char *value, t_word_type type);
void						add_command_part_to_list(t_command_part **lst,
								t_command_part *new);
void						handle_token_join(t_tokenizer_ctx *ctx);
void						handle_token_expansion(t_word_type previous_word_type,
								t_command_part **command_node, t_shell *shell);

#endif

// space //letter // end // single quote //double quote //redirect IN
// redirect out