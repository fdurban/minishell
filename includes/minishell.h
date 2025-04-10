/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:10:39 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/10 16:49:19 by fdurban-         ###   ########.fr       */
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
# include <stdio.h>`
# include <string.h>

typedef enum estate
{
    STATE_START,
    STATE_IN_SINGLE_QUOTE,
    STATE_IN_DOUBLE_QUOTE,
    STATE_PIPE,
    STATE_REDIR,
    STATE_END,
    STATE_ERROR,
    NUM_STATES
} t_state;
typedef enum einput
{
    INPUT_LETTER,
    INPUT_SPACE,
    INPUT_END,
    INPUT_IN_SINGLE_QUOTE,
    INPUT_IN_DOUBLE_QUOTE,
    INPUT_PIPE,
    INPUT_REDIRECT,
    
} t_input;

void	shell_loop(t_env *env);
#endif