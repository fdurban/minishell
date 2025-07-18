/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakul <yakul@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:10:39 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/18 12:22:27 by yakul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "builtins.h"
# include "env.h"
# include "execution.h"
# include "shell_loop.h"
# include "tokenizer.h"

/* Standard libraries */
# include <errno.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

/* Readline for input */
# include <readline/history.h>
# include <readline/readline.h>

/* Libft (if you use it) */
# include "../libft/get_next_line/get_next_line.h"
# include "../libft/printf/ft_printf.h"
# include "../libft/standard/libft.h"
# include <linux/limits.h>
# include <stdio.h>
# include <string.h>

extern sig_atomic_t	g_signal_state;

#endif