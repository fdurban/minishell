/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:35:16 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/24 10:13:49 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_env *env_copy;

	(void)argc;
	(void)argv;
	env_copy = copy_env(envp);
	char *tokens[] = {ft_strdup("echo"), ft_strdup("Hello"), ft_strdup("World"),
		ft_strdup("|"), ft_strdup("tr"), ft_strdup("a-z"), ft_strdup("A-Z"),
		ft_strdup("|"), ft_strdup("wc"), ft_strdup("-w"), NULL};
	int exit_status;

	exit_status = execution(tokens, env_copy);
	printf("Execution returned exit status: %d\n", exit_status);

	// Free the tokens manually allocated.
	free(tokens[0]);
	free(tokens[1]);

	return (exit_status);
}