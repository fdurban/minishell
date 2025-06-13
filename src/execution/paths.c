/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 11:27:23 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/13 09:18:03 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*find_command_path(char **paths, char *cmd)
{
	int		j;
	char	*full_path;

	j = 0;
	while (paths[j])
	{
		full_path = join_path_cmd(paths[j], cmd);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		j++;
	}
	return (NULL);
}

void	resolve_command_paths(t_pipex *pipex, char **paths)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count)
	{
		if (!pipex->cmd_args[i][0] || pipex->cmd_args[i][0][0] == '\0')
			pipex->cmd_paths[i] = NULL;
		else if (access(pipex->cmd_args[i][0], X_OK) == 0)
			pipex->cmd_paths[i] = ft_strdup(pipex->cmd_args[i][0]);
		else
		{
			pipex->cmd_paths[i] = find_command_path(paths,
					pipex->cmd_args[i][0]);
		}
		i++;
	}
	pipex->cmd_paths[i] = NULL;
}

void	parse_paths(t_pipex *pipex, t_shell *shell)
{
	char	*path_env;
	char	**paths;
	int		i;

	path_env = get_env_var(shell->env, "PATH");
	if (path_env)
		paths = ft_split(path_env, ':');
	else
	{
		paths = malloc(sizeof(char *));
		if (!paths)
			handle_error("malloc paths fallback");
		paths[0] = NULL;
	}
	pipex->cmd_paths = malloc(sizeof(char *) * (pipex->cmd_count + 1));
	if (!pipex->cmd_paths)
		handle_error("Error: Memory allocation failed for cmd_paths");
	resolve_command_paths(pipex, paths);
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}
