/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:20:08 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/16 15:18:15 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_hostname(t_env *env)
{
	char	*sessionmanager;
	char	*start;
	char	*end;
	char	*hostname;
	char	*dot;

	sessionmanager = get_env_var(env, "SESSION_MANAGER");
	if (!sessionmanager)
		return (ft_strdup(""));
	start = ft_strchr(sessionmanager, '/');
	end = ft_strchr(sessionmanager, ':');
	if (!start || !end || end <= start)
		return (ft_strdup(""));
	hostname = ft_substr(start + 1, 0, (end - start) - 1);
	dot = ft_strchr(hostname, '.');
	if (dot)
		*dot = '\0';
	return (hostname);
}

static int	handle_signal_interrupt(char *input, t_shell *shell)
{
	if (!input)
	{
		shell->exit_status = 130;
		g_signal_state = 0;
		return (0);
	}
	if (g_signal_state == SIGINT)
	{
		shell->exit_status = 130;
		g_signal_state = 0;
		if (!input)
			return (1);
	}
	return (0);
}

static int	handle_null_or_empty_input(char *input)
{
	if (!input)
		return (1);
	if (*input == '\0')
	{
		free(input);
		return (2);
	}
	return (0);
}

static void	run_command(char *input, t_shell *shell)
{
	shell->state = SHELL_EXEC;
	set_signal_handlers(SHELL_EXEC);
	process_command_line(input, shell);
	free(input);
}

void	shell_loop(t_shell *shell)
{
	char	*input;
	int		result;

	while (1)
	{
		shell->state = SHELL_MAIN;
		g_signal_state = 0;
		set_signal_handlers(SHELL_MAIN);
		input = get_user_input(shell->env);
		if (handle_signal_interrupt(input, shell))
			continue ;
		result = handle_null_or_empty_input(input);
		if (result == 1)
		{
			printf("exit\n");
			break ;
		}
		if (result == 2)
			continue ;
		run_command(input, shell);
	}
}
