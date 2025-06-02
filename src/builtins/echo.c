/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:29:56 by igngonza          #+#    #+#             */
/*   Updated: 2025/05/13 15:56:07 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*replace_exit_status(const char *arg, int exit_status)
{
	char	*result;
	char	*exit_status_str;
	size_t	len_before;
	size_t	len_after;
	const char	*pos;

	// Find "$?" manually using a while loop
	pos = NULL;
	size_t i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] == '$' && arg[i + 1] == '?')
		{
			pos = &arg[i];
			break;
		}
		i++;
	}
	if (!pos)
		return (ft_strdup(arg));
	exit_status_str = ft_itoa(exit_status);
	if (!exit_status_str)
		return (NULL);
	len_before = pos - arg;
	len_after = ft_strlen(pos + 2);
	result = malloc(len_before + ft_strlen(exit_status_str) + len_after + 1);
	if (!result)
	{
		free(exit_status_str);
		return (NULL);
	}

	ft_memcpy(result, arg, len_before);
	ft_memcpy(result + len_before, exit_status_str, ft_strlen(exit_status_str));
	ft_memcpy(result + len_before + ft_strlen(exit_status_str), pos + 2, len_after);
	result[len_before + ft_strlen(exit_status_str) + len_after] = '\0';

	free(exit_status_str);
	return (result);
}

int	builtin_echo(char **args, t_shell *shell)
{
	int		newline;
	char	*expanded_arg;
	int		i;

	i = 1;
	newline = 1;
	(void)shell;
	if (args[1] && ft_strncmp(args[1], "-n", 2) == 0)
	{
		newline = 0;
		i++;
	}

	while (args[i])
	{
		expanded_arg = replace_exit_status(args[i], shell->exit_status);
		if (expanded_arg)
		{
			ft_putstr_fd(expanded_arg, STDOUT_FILENO);
			free(expanded_arg);
		}
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}