/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 18:48:08 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/24 09:51:28 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>

int			g_in_prompt = 1;

static void	sigint_handler(int sig)
{
	(void)sig;
	if (g_in_prompt)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
	{
		write(1, "\n", 1);
	}
}

static void	sigquit_handler(int sig)
{
	(void)sig;
	write(1, "\b\b  \b\b", 6);
}

void	init_signal_handlers(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	signal(SIGQUIT, SIG_IGN);
}
