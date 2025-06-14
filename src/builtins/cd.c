/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:56:30 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/13 09:42:40 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*duplicate_str(const char *s)
{
	size_t	len;
	size_t	i;
	char	*dup;

	len = ft_strlen(s);
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

static char	*join_paths(const char *base, const char *relative)
{
	size_t	base_len;
	size_t	rel_len;
	char	*full_path;
	char	*dest;

	base_len = ft_strlen(base);
	rel_len = ft_strlen(relative);
	full_path = malloc(base_len + rel_len + 2);
	if (!full_path)
		return (NULL);
	dest = full_path;
	while (*base)
		*dest++ = *base++;
	*dest++ = '/';
	while (*relative)
		*dest++ = *relative++;
	*dest = '\0';
	return (full_path);
}

static char	*get_abs_path(const char *arg, t_shell *shell)
{
	char	*abs_path;
	char	*pwd;
	char	*special;

	special = handle_dot_paths(arg, shell);
	if (special)
		return (special);
	if (arg[0] == '/')
		abs_path = duplicate_str(arg);
	else
	{
		pwd = get_env_var(shell->env, "PWD");
		if (!pwd)
			return (NULL);
		abs_path = join_paths(pwd, arg);
	}
	return (abs_path);
}

static int	change_directory(const char *arg, t_shell *shell)
{
	char	*path;

	path = get_abs_path(arg, shell);
	if (!path)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd((char *)arg, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		shell->exit_status = 1;
		return (0);
	}
	if (chdir(path) == 0)
	{
		update_env_field(shell->env, "PWD", path);
		free(path);
		return (1);
	}
	else
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd((char *)arg, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		shell->exit_status = 1;
		free(path);
		return (0);
	}
}

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*home;

	if (!argv[1])
	{
		home = get_env_var(shell->env, "HOME");
		if (!home)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			shell->exit_status = 1;
			return (1);
		}
		if (!change_directory(home, shell))
		{
			free(home);
			return (1);
		}
		free(home);
		return (0);
	}
	if (change_directory(argv[1], shell))
		return (0);
	return (1);
}
