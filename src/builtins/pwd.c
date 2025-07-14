/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:42:16 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/14 12:28:54 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_pwd(char **args, t_shell *shell)
{
	int		length;
	int		i;
	char	*pwd;

	(void)args;
	i = 0;
	pwd = get_env_var(shell->env, "PWD");
	if (!pwd)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			perror("getcwd");
			return (1);
		}
	}
	length = ft_strlen(pwd);
	while (i < length)
	{
		write(1, &pwd[i], 1);
		i++;
	}
	write(1, "\n", 1);
	free(pwd);
	return (0);
}
