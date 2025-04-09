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

void	shell_loop(t_env *env);
#endif