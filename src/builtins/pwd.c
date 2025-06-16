/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:42:16 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/16 16:18:48 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_pwd(char **args, t_shell *shell)
{
	int length;
	int i;
	char *pwd;

	(void)args;
	pwd = get_env_var(shell->env, "PWD");
	if (access(pwd, R_OK) != 0)
	{
		ft_putstr_fd("pwd: is a directory\n", STDERR_FILENO);
		printf("hello there\n");
		shell->exit_status = 126;
		return (126);
	}
	length = ft_strlen(pwd);
	for (i = 0; i < length; i++)
	{
		write(1, &pwd[i], 1);
	}
	write(1, "\n", 1);
	return (0);
}