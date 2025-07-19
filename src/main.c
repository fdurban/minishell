/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:11:03 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/19 13:57:49 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.exit_status = 0;
	shell.env = copy_env(envp);
	shell.pwd = getcwd(NULL, 0);
	shell.oldpwd = NULL;
	if (!shell.env)
	{
		printf("Failed to copy environment\n");
		return (1);
	}
	shell_loop(&shell);
	free_env(shell.env);
	rl_clear_history();
	exit(shell.exit_status);
}
