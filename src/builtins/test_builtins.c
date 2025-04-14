/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:19:11 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/14 10:22:08 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"
#include "../../includes/env.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char **argv, char **envp)
{
	t_shell shell;
	t_env *env;
	char *test1[] = {"echo", "hello", "world!", NULL};
	char *test2[] = {"echo", "-n", "no", "newline", NULL};
	char *test3[] = {"echo", NULL};
	char *test4[] = {"cd", "/home/yakul/Developer/pipex", NULL};
	char *test5[] = {"cd", "includes", NULL};
	char *reset[] = {"cd", NULL};

	(void)argc;
	(void)argv;
	env = copy_env(envp);
	if (!env)
	{
		fprintf(stderr, "Failed to copy env\n");
		return (1);
	}
	shell.env = env;

	printf("Test 1:\n");
	if (is_builtin(test1[0]))
		exec_builtin(test1, &shell);
	printf("\n\n");

	printf("Test 2:\n");
	if (is_builtin(test2[0]))
		exec_builtin(test2, &shell);
	printf(" <- (no newline)\n\n");

	printf("Test 3:\n");
	if (is_builtin(test3[0]))
		exec_builtin(test3, &shell);
	printf("\n\n");

	printf("Test 4:\n");
	char *result = get_env_var(shell.env, "PWD");
	printf("the shell: %s\n", result);
	if (is_builtin(test4[0]))
		exec_builtin(test4, &shell);
	result = get_env_var(shell.env, "PWD");
	printf("pwd: %s\n", result);
	printf("\n\n");

	exec_builtin(reset, &shell);

	printf("Test 5:\n");
	result = get_env_var(shell.env, "PWD");
	printf("pwd: %s\n", result);
	if (is_builtin(test5[0]))
		exec_builtin(test5, &shell);
	result = get_env_var(shell.env, "PWD");
	printf("the shell: %s\n", result);
	printf("\n\n");

	free_env(env);
	return (0);
}