/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:32:46 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/19 12:35:40 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"
#include "../../includes/minishell.h"

static t_builtin	*get_builtins(void)
{
	static t_builtin	builtins[] = {{"echo", builtin_echo}, {"cd",
		builtin_cd}, {"pwd", builtin_pwd}, {"export", builtin_export},
	{"unset", builtin_unset}, {"env", builtin_env}, {"exit",
		builtin_exit}, {NULL, NULL}};

	return (builtins);
}

int	is_builtin(const char *cmd)
{
	int			i;
	t_builtin	*builtins;
	size_t		cmd_len;
	size_t		builtin_len;

	i = 0;
	builtins = get_builtins();
	cmd_len = ft_strlen(cmd);
	while (builtins[i].name)
	{
		builtin_len = ft_strlen(builtins[i].name);
		if (cmd_len == builtin_len && ft_strncmp(builtins[i].name, cmd,
				cmd_len) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	exec_builtin(char **args, t_shell *shell)
{
	int			i;
	t_builtin	*builtins;
	size_t		arg_len;
	size_t		builtin_len;

	i = 0;
	builtins = get_builtins();
	arg_len = ft_strlen(args[0]);
	while (builtins[i].name)
	{
		builtin_len = ft_strlen(builtins[i].name);
		if (arg_len == builtin_len && ft_strncmp(builtins[i].name, args[0],
				arg_len) == 0)
			return (builtins[i].func(args, shell));
		i++;
	}
	return (-1);
}
