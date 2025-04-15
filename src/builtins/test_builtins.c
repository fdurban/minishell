/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:19:11 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/15 15:20:28 by igngonza         ###   ########.fr       */
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
	char *test6[] = {"cd", ".", NULL};
	char *test7[] = {"cd", "..", NULL};
	char *test8[] = {"pwd", NULL};
	char *test9[] = {"export", NULL};
	char *test10[] = {"export", "0badkey=value", NULL};
	char *test11[] = {"export", "GOODKEY=hellothere", NULL};
	char *test12[] = {"export", "GOODKEY", NULL};
	char *test13[] = {"export", "GOODKEY=changed", NULL};
	char *test14[] = {"export", "Astro", NULL};
	char *test15[] = {"unset", "Astro", NULL};
	char *reset[] = {"cd", "/home/yakul/Developer/minishell", NULL};
	char *result;

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
	result = get_env_var(shell.env, "PWD");
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
	exec_builtin(reset, &shell);
	printf("Test 6:\n");
	result = get_env_var(shell.env, "PWD");
	printf("pwd: %s\n", result);
	if (is_builtin(test6[0]))
		exec_builtin(test6, &shell);
	result = get_env_var(shell.env, "PWD");
	printf("the shell: %s\n", result);
	printf("\n\n");
	exec_builtin(reset, &shell);
	printf("Test 7:\n");
	result = get_env_var(shell.env, "PWD");
	printf("pwd: %s\n", result);
	if (is_builtin(test7[0]))
		exec_builtin(test7, &shell);
	result = get_env_var(shell.env, "PWD");
	printf("the shell: %s\n", result);
	printf("\n\n");
	exec_builtin(reset, &shell);
	printf("Test 8:\n");
	result = get_env_var(shell.env, "PWD");
	printf("pwd: %s\n", result);
	if (is_builtin(test8[0]))
		exec_builtin(test8, &shell);
	printf("\n\n");
	printf("Test 9:\n");
	printf("pwd: %s\n", result);
	if (is_builtin(test9[0]))
		exec_builtin(test9, &shell);
	printf("\n\n");

	printf("Test 10:\n");
	if (is_builtin(test10[0]))
	{
		exec_builtin(test10, &shell);
		exec_builtin(test9, &shell);
	}
	printf("\n\n");

	printf("Test 11:\n");
	if (is_builtin(test11[0]))
	{
		exec_builtin(test11, &shell);
		exec_builtin(test9, &shell);
	}
	printf("\n\n");

	printf("Test 12:\n");
	if (is_builtin(test12[0]))
	{
		exec_builtin(test12, &shell);
		exec_builtin(test9, &shell);
	}
	printf("\n\n");

	printf("Test 13:\n");
	if (is_builtin(test13[0]))
	{
		exec_builtin(test13, &shell);
		exec_builtin(test9, &shell);
	}
	printf("\n\n");

	printf("Test 14:\n");
	if (is_builtin(test14[0]))
	{
		exec_builtin(test14, &shell);
		exec_builtin(test9, &shell);
	}
	printf("\n\n");

	printf("Test 15:\n");
	if (is_builtin(test15[0]))
	{
		exec_builtin(test15, &shell);
		exec_builtin(test9, &shell);
	}
	printf("\n\n");

	free_env(env);
	return (0);
}