/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 18:48:08 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/18 17:38:54 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <sys/ioctl.h>

sig_atomic_t	g_signal_state = 0;

void	sigint_prompt_handler(int sig)
{
	(void)sig;
	g_signal_state = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_heredoc_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

void	sigint_exec_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

void	set_signal_handlers(int mode)
{
	if (mode == SHELL_MAIN)
	{
		signal(SIGINT, sigint_prompt_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == SHELL_HEREDOC)
	{
		signal(SIGINT, sigint_heredoc_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == SHELL_EXEC)
	{
		signal(SIGINT, sigint_exec_handler);
		signal(SIGQUIT, SIG_IGN);
	}
}
