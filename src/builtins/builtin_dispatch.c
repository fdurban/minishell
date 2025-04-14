/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:32:46 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/14 11:48:47 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"

static t_builtin	*get_builtins(void)
{
	static t_builtin	builtins[] = {{"echo", builtin_echo}, {"cd",
			builtin_cd}, {"pwd", builtin_pwd}, {NULL, NULL}};

	return (builtins);
}

int	is_builtin(const char *cmd)
{
	int			i;
	t_builtin	*builtins;

	i = 0;
	builtins = get_builtins();
	while (builtins[i].name)
	{
		if (strcmp(builtins[i].name, cmd) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	exec_builtin(char **args, t_shell *shell)
{
	int			i;
	t_builtin	*builtins;

	i = 0;
	builtins = get_builtins();
	while (builtins[i].name)
	{
		if (strcmp(builtins[i].name, args[0]) == 0)
			return (builtins[i].func(args, shell));
		i++;
	}
	return (-1);
}
