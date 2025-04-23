/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:10:39 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/23 14:06:59 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "env.h"

/* Standard libraries */
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>

/* Readline for input */
# include <readline/history.h>
# include <readline/readline.h>

/* Libft (if you use it) */
# include "../libft/standard/libft.h"
# include <stdio.h>
# include <string.h>

typedef enum e_state
{
	START,
	WORD,
	IN_SINGLE_QUOTE,
	IN_DOUBLE_QUOTE,
	PIPE,
	REDIR,
	SPACE_AFTER_WORD,
	END_OF_SINGLE_QUOTE,
	END_OF_DOUBLE_QUOTE,
	END,
	ERROR,
	NUM_STATES
}	t_state;

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


typedef struct s_command_part
{
	char	*value;
	t_input	type;
	struct s_command_part	*next;
} t_command_part;

void	shell_loop(t_env *env);
int		parse(char *readline);
int		get_input_type(char c);
char**	tokenize(char *valid_input, int token_number);

#endif