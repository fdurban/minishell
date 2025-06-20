/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:11:03 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/20 09:48:57 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <editline/readline.h>

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.exit_status = 0;
	shell.env = copy_env(envp);
	if (!shell.env)
	{
		fprintf(stderr, "Failed to copy environment\n");
		return (1);
	}
	init_signal_handlers();
	shell_loop(&shell);
	free_env(shell.env);
	rl_clear_history();
	return (shell.exit_status);
}
