/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:10:55 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/24 10:06:39 by igngonza         ###   ########.fr       */
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
	safe_close(&pipex->in_fd);
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
	i = 0;
	while (i < total_fds)
	{
		close(pipex->pipes[i]);
		i++;
	}
}

void	execute_child_command(t_pipex *pipex, t_env *envp)
{
	char	*cmd;
	char	**cmd_args;

	cmd = pipex->cmd_paths[pipex->idx];
	cmd_args = pipex->cmd_args[pipex->idx];
	if (execve(cmd, cmd_args, envp->vars) == -1)
		perror("execve failed");
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

void	create_child_process(t_pipex *pipex, t_env *envp)
{
	int	saved_stdout;

	pipex->pid = fork();
	if (pipex->pid == -1)
		handle_error("Fork failed");
	if (pipex->pid == 0)
	{
		if (pipex->cmd_count > 1)
		{
			saved_stdout = dup(STDOUT_FILENO);
			setup_child_io(pipex);
			close_pipes(pipex);
			handle_child_error(pipex, saved_stdout);
		}
		fprintf(stderr, "Child process: executing %s\n",
			pipex->cmd_paths[pipex->idx]);
		execute_child_command(pipex, envp);
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
	// Allocate an array of pointers to command argument arrays.
	pipex->cmd_args = malloc(sizeof(char **) * (n_cmds + 1));
	if (!pipex->cmd_args)
		handle_error("Memory allocation failed for cmd_args");
	cmd_idx = 0;
	i = 0;
	while (tokens[i])
	{
		// Count the number of arguments for the current command.
		arg_count = 0;
		start = i;
		while (tokens[i] && ft_strcmp(tokens[i], "|") != 0)
		{
			arg_count++;
			i++;
		}
		// Allocate the argument array for this command.
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
		// If there is a pipe token, skip it.
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

int	execution(char **tokens, t_env *env_copy)
{
	t_pipex pipex;
	int status;
	int last_exit_status;
	int last_exit_id;

	ft_bzero(&pipex, sizeof(t_pipex));
	last_exit_status = 0;
	parse_cmds(&pipex, tokens);
	parse_paths(&pipex, env_copy);
	if (pipex.cmd_count > 1)
	{
		pipex.pipe_count = pipex.cmd_count - 1;
		pipex.pipes = malloc(sizeof(int) * 2 * (pipex.pipe_count));
		if (!pipex.pipes)
			handle_error("Memory allocation failed for pipes");
		create_pipes(&pipex);
	}
	else
	{
		pipex.in_fd = STDIN_FILENO;
		pipex.out_fd = STDOUT_FILENO;
	}
	pipex.idx = -1;
	while (++(pipex.idx) < pipex.cmd_count)
	{
		printf("Creating child process for command %d\n", pipex.idx);
		create_child_process(&pipex, env_copy);
	}
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