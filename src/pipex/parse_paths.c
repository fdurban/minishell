/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_paths.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:42:53 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/21 13:56:19 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*join_path_cmd(char *dir, char *cmd)
{
	char	*temp;
	char	*full_path;

	if (!dir || !cmd)
		return (NULL);
	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

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

void	parse_paths(t_pipex *pipex, t_env *envp)
{
	char	*path_env;
	char	**paths;
	int		i;

	path_env = get_env_var(envp, "PATH");
	if (!path_env)
		handle_error("Error: PATH not found");
	paths = ft_split(path_env, ':');
	if (!paths)
		handle_error("Error: Failed to split PATH");
	pipex->cmd_paths = malloc(sizeof(char *) * (pipex->cmd_count + 1));
	if (!pipex->cmd_paths)
		handle_error("Error: Memory allocation failed for cmd_paths");
	resolve_command_paths(pipex, paths);
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}
