/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:56:30 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/14 11:11:30 by igngonza         ###   ########.fr       */
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

static int	change_directory(const char *path, t_shell *shell)
{
	if (chdir(path) == 0)
	{
		update_env_field(shell->env, "PWD", path);
		return (1);
	}
	return (0);
}

int	builtin_cd(char **argv, t_shell *shell)
{
	int		i;
	char	*path;
	int		result;

	i = 1;
	result = 0;
	while (argv[i])
	{
		path = get_abs_path(argv[i], shell);
		if (!path)
			return (0);
		if (change_directory(path, shell))
		{
			result = 1;
			free(path);
			return (result);
		}
		free(path);
		i++;
	}
	return (result);
}
