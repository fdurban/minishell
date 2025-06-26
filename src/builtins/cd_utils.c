/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:58:38 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/26 11:20:22 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*handle_single_dot(t_shell *shell)
{
	char	*pwd;

	pwd = get_env_var(shell->env, "PWD");
	if (!pwd)
		return (NULL);
	return (ft_strdup(pwd));
}

static char	*handle_double_dot(t_shell *shell)
{
	char	*pwd;
	char	*new_pwd;
	char	*last_slash;
	size_t	len;

	pwd = get_env_var(shell->env, "PWD");
	if (!pwd)
		return (NULL);
	new_pwd = ft_strdup(pwd);
	if (!new_pwd)
		return (NULL);
	len = ft_strlen(new_pwd);
	if (len > 1 && new_pwd[len - 1] == '/')
		new_pwd[len - 1] = '\0';
	last_slash = ft_strrchr(new_pwd, '/');
	if (last_slash)
		*last_slash = '\0';
	if (ft_strlen(new_pwd) == 0)
	{
		free(new_pwd);
		new_pwd = ft_strdup("/");
	}
	return (new_pwd);
}

char	*handle_dot_paths(const char *arg, t_shell *shell)
{
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
