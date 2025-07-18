/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:58:38 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/18 17:56:16 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_abs_path(const char *arg, t_shell *shell)
{
	char	*special;
	char	*normalized;
	char	*abs_path;

	if (ft_strcmp(arg, "-") == 0)
		return (handle_dash(shell));
	if (ft_strcmp(arg, "~") == 0)
		return (handle_tilde(shell));
	special = handle_dot_paths(arg, shell);
	if (special)
		return (special);
	normalized = collapse_slashes(arg);
	if (!normalized)
		return (NULL);
	abs_path = resolve_normal_path(normalized, shell);
	free(normalized);
	return (abs_path);
}

static char	*handle_single_dot(t_shell *shell)
{
	char	cwd[PATH_MAX];
	char	*env_pwd;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		if (shell->pwd && ft_strlen(shell->pwd) > 0)
			return (ft_strdup(shell->pwd));
		else
		{
			env_pwd = get_env_var(shell->env, "PWD");
			if (env_pwd)
				return (ft_strdup(env_pwd));
			return (NULL);
		}
	}
	return (ft_strdup(cwd));
}

char	*handle_dot_paths(const char *arg, t_shell *shell)
{
	(void)shell;
	if (strcmp(arg, ".") == 0)
		return (handle_single_dot(shell));
	else if (strcmp(arg, "..") == 0)
		return (handle_double_dot(shell));
	return (NULL);
}

char	*collapse_slashes(const char *path)
{
	int		i;
	int		j;
	char	*collapsed;

	i = 0;
	j = 0;
	collapsed = malloc(ft_strlen(path) + 1);
	if (!collapsed)
		return (NULL);
	while (path[i])
	{
		collapsed[j++] = path[i];
		if (path[i] == '/')
		{
			while (path[i] == '/')
				i++;
		}
		else
			i++;
	}
	while (j > 1 && collapsed[j - 1] == '/')
		j--;
	collapsed[j] = '\0';
	return (collapsed);
}
