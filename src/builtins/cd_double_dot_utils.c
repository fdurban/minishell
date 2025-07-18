/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_double_dot_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakul <yakul@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:05:32 by yakul             #+#    #+#             */
/*   Updated: 2025/07/18 12:17:59 by yakul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_fallback_pwd(t_shell *shell)
{
	char	cwd[PATH_MAX];
	char	*env_pwd;

	if (getcwd(cwd, sizeof(cwd)))
		return (ft_strdup(cwd));
	if (shell->pwd && *shell->pwd)
		return (ft_strdup(shell->pwd));
	env_pwd = get_env_var(shell->env, "PWD");
	if (env_pwd)
		return (ft_strdup(env_pwd));
	return (NULL);
}

static void	trim_last_path_component(char *path)
{
	size_t	len;
	char	*last_slash;

	len = ft_strlen(path);
	if (len > 1 && path[len - 1] == '/')
		path[len - 1] = '\0';
	last_slash = ft_strrchr(path, '/');
	if (last_slash)
		*last_slash = '\0';
	if (ft_strlen(path) == 0)
		ft_strlcpy(path, "/", PATH_MAX);
}

static void	fix_to_existing_parent(char *path)
{
	char	*last_slash;

	while (ft_strlen(path) > 1 && access(path, F_OK) != 0)
	{
		last_slash = ft_strrchr(path, '/');
		if (!last_slash)
			break ;
		*last_slash = '\0';
		if (ft_strlen(path) == 0)
		{
			ft_strlcpy(path, "/", PATH_MAX);
			break ;
		}
	}
}

char	*handle_double_dot(t_shell *shell)
{
	char	*new_pwd;
	char	temp[PATH_MAX];

	new_pwd = get_fallback_pwd(shell);
	if (!new_pwd)
		return (NULL);
	trim_last_path_component(new_pwd);
	ft_strlcpy(temp, new_pwd, PATH_MAX);
	if (access(temp, F_OK) != 0)
		fix_to_existing_parent(new_pwd);
	return (new_pwd);
}
