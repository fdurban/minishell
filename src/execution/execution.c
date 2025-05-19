/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:10:55 by igngonza          #+#    #+#             */
/*   Updated: 2025/05/19 18:34:39 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

void	parent_free(t_pipex *pipex)
{
	if (!pipex)
		return ;
	if (pipex->in_fd > STDERR_FILENO)
		safe_close(&pipex->in_fd);
	if (pipex->out_fd > STDERR_FILENO)
		safe_close(&pipex->out_fd);
	cleanup_heredoc(pipex);
	free_cmd_paths(pipex);
	free_cmd_args(pipex);
	if (pipex->pipes)
	{
		free(pipex->pipes);
		pipex->pipes = NULL;
	}
}

void	close_pipes(t_pipex *pipex)
{
	int	i;
	int	total_fds;

	total_fds = pipex->pipe_count * 2;
	for (i = 0; i < total_fds; i++)
		close(pipex->pipes[i]);
}

void	execute_child_command(t_pipex *pipex, t_env *envp)
{
	char	*cmd;
	char	**cmd_args;

	cmd = pipex->cmd_paths[pipex->idx];
	cmd_args = pipex->cmd_args[pipex->idx];
	if (execve(cmd, cmd_args, envp->vars) == -1)
	{
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
}
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

void	create_child_process(t_pipex *px, t_shell *shell)
{
	pid_t	pid;
	char	**cmd;

	cmd = NULL;
	pid = fork();
	if (pid < 0)
		handle_error("fork failed");
	px->pids[px->idx] = pid;
	if (pid == 0)
	{
		if (px->cmd_count > 1)
		{
			if (px->idx == 0)
				dup2(px->pipes[1], STDOUT_FILENO);
			else if (px->idx == px->cmd_count - 1)
				dup2(px->pipes[2 * (px->idx - 1)], STDIN_FILENO);
			else
			{
				dup2(px->pipes[2 * (px->idx - 1)], STDIN_FILENO);
				dup2(px->pipes[2 * px->idx + 1], STDOUT_FILENO);
			}
		}
		close_pipes(px);
		cmd = px->cmd_args[px->idx];
		if (is_builtin(cmd[0]))
		{
			exec_builtin(cmd, shell);
			exit(shell->exit_status);
			exit(0);
		}
		else
		{
			execute_child_command(px, shell->env);
			perror(cmd[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (px->cmd_count > 1)
		{
			if (px->idx == 0)
				close(px->pipes[1]);
			else if (px->idx == px->cmd_count - 1)
				close(px->pipes[2 * (px->idx - 1)]);
			else
			{
				close(px->pipes[2 * (px->idx - 1)]);
				close(px->pipes[2 * px->idx + 1]);
			}
		}
	}
}

void	create_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count - 1)
	{
		if (pipe(pipex->pipes + 2 * i) < 0)
			parent_free(pipex);
		i++;
	}
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

void	parse_cmds(t_pipex *pipex, char **tokens)
{
	int	i;
	int	n_cmds;
	int	cmd_idx;
	int	arg_count;
	int	j;
	int	start;

	n_cmds = 1;
	i = 0;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "|") == 0)
			n_cmds++;
		i++;
	}
	pipex->cmd_count = n_cmds;
	pipex->cmd_args = malloc(sizeof(char **) * (n_cmds + 1));
	if (!pipex->cmd_args)
		handle_error("Memory allocation failed for cmd_args");
	cmd_idx = 0;
	i = 0;
	while (tokens[i])
	{
		arg_count = 0;
		start = i;
		while (tokens[i] && ft_strcmp(tokens[i], "|") != 0)
		{
			arg_count++;
			i++;
		}
		pipex->cmd_args[cmd_idx] = malloc(sizeof(char *) * (arg_count + 1));
		if (!pipex->cmd_args[cmd_idx])
			handle_error("Memory allocation failed for a command");
		j = 0;
		while (j < arg_count)
		{
			pipex->cmd_args[cmd_idx][j] = ft_strdup(tokens[start + j]);
			j++;
		}
		pipex->cmd_args[cmd_idx][arg_count] = NULL;
		cmd_idx++;
		if (tokens[i] && ft_strcmp(tokens[i], "|") == 0)
			i++;
	}
	pipex->cmd_args[cmd_idx] = NULL;
}

void	*ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;

	ptr = s;
	while (n--)
		*ptr++ = 0;
	return (s);
}

void	init_pipex_pids(t_pipex *pipex)
{
	pipex->pids = malloc(sizeof(pid_t) * pipex->cmd_count);
	if (!pipex->pids)
		handle_error("Failed to allocate memory for pids");
}

void	execute_pipeline(t_pipex *pipex, t_shell *shell)
{
	int	i;

	init_pipex_pids(pipex);
	for (i = 0; i < pipex->cmd_count; i++)
	{
		pipex->idx = i;
		create_child_process(pipex, shell);
		pipex->pids[i] = pipex->pid;
	}
	close_pipes(pipex);
	for (i = 0; i < pipex->cmd_count; i++)
	{
		if (waitpid(pipex->pids[i], NULL, 0) == -1)
			handle_error("waitpid failed");
	}
	free(pipex->pids);
}

int	execution(char **tokens, t_shell *shell)
{
	t_pipex	pipex;
	int		status;
	int		last_exit_status;
	int		last_exit_id;
	size_t	bytes;

	ft_bzero(&pipex, sizeof(pipex));
	last_exit_status = 0;
	parse_cmds(&pipex, tokens);
	parse_paths(&pipex, shell);
	if (pipex.cmd_count > 1)
	{
		pipex.pipe_count = pipex.cmd_count - 1;
		bytes = sizeof(int) * 2 * pipex.pipe_count;
		pipex.pipes = malloc(bytes);
		if (!pipex.pipes)
			handle_error("Memory allocation failed for pipes");
		create_pipes(&pipex);
	}
	else
	{
		pipex.in_fd = STDIN_FILENO;
		pipex.out_fd = STDOUT_FILENO;
	}
	init_pipex_pids(&pipex);
	pipex.idx = -1;
	while (++pipex.idx < pipex.cmd_count)
		create_child_process(&pipex, shell);
	close_pipes(&pipex);
	last_exit_id = waitpid(-1, &status, 0);
	while (last_exit_id > 0)
	{
		if (WIFEXITED(status) && pipex.pid == last_exit_id)
			last_exit_status = WEXITSTATUS(status);
		last_exit_id = waitpid(-1, &status, 0);
	}
	parent_free(&pipex);
	return (last_exit_status);
}
