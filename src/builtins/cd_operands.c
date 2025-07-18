/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_operands.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:30:59 by yakul             #+#    #+#             */
/*   Updated: 2025/07/18 17:55:09 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*duplicate_str(const char *s)
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

char	*join_paths(const char *base, const char *relative)
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

char	*handle_dash(t_shell *shell)
{
	if (!shell->oldpwd || ft_strlen(shell->oldpwd) == 0)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
		return (NULL);
	}
	ft_putendl_fd(shell->oldpwd, STDOUT_FILENO);
	return (ft_strdup(shell->oldpwd));
}

char	*handle_tilde(t_shell *shell)
{
	char	*home;

	home = get_env_var(shell->env, "HOME");
	if (!home)
	{
		ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		return (NULL);
	}
	return (ft_strdup(home));
}

char	*resolve_normal_path(const char *normalized, t_shell *shell)
{
	char	cwd[PATH_MAX];

	if (normalized[0] == '/')
		return (duplicate_str(normalized));
	if (!getcwd(cwd, sizeof(cwd)))
	{
		if (shell->pwd && *shell->pwd)
			return (join_paths(shell->pwd, normalized));
		ft_putstr_fd("cd: getcwd failed and no PWD available\n", STDERR_FILENO);
		return (NULL);
	}
	return (join_paths(cwd, normalized));
}
