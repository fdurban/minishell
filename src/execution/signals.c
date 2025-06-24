/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 18:48:08 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/24 12:32:41 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>

volatile sig_atomic_t	g_signal_state = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	if (g_signal_state == 1)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_signal_state == 2)
	{
		write(1, "\n", 1);
		close(STDIN_FILENO);
	}
	else
	{
		write(1, "\n", 1);
	}
}

void	init_signal_handlers(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
