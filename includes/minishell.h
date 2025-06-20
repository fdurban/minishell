/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:10:39 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/20 09:48:20 by igngonza         ###   ########.fr       */
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
# include <stdio.h>
# include <string.h>

#endif