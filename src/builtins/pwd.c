/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:42:16 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/14 12:21:07 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_pwd(char **args, t_shell *shell)
{
	int length;
	int i;
	char *pwd;
	(void)args;

	i = 0;
	length = ft_strlen(get_env_var(shell->env, "PWD"));
	pwd = ft_strdup(get_env_var(shell->env, "PWD"));
	while (i < length)
	{
		write(1, &pwd[i], 1);
		i++;
	}
	write(1, "\n", 1);
	return (0);
}