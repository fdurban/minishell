#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdio.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/standard/libft.h"

typedef enum estate
{
    STATE_START,
    STATE_IN_QUOTE,
    STATE_END,
    STATE_ERROR,
    NUM_STATES
} t_state;

#endif