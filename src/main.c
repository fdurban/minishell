/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:11:03 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/09 20:11:04 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_copy;

	(void)argc;
	(void)argv;
	env_copy = copy_env(envp);
	if (!env_copy)
	{
		fprintf(stderr, "Failed to copy environment\n");
		return (1);
	}
	shell_loop(env_copy);
	free_env(env_copy);
	return (0);
}
