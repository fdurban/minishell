/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:56:30 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/18 17:59:25 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_cd_error(const char *arg, char *path, t_shell *shell)
{
	ft_putstr_fd("cd: ", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	shell->exit_status = 1;
	free(path);
	return (0);
}

static void	update_pwd_and_oldpwd(t_shell *shell)
{
	char	cwd[PATH_MAX];

	if (shell->pwd)
	{
		free(shell->oldpwd);
		shell->oldpwd = ft_strdup(shell->pwd);
		free(shell->pwd);
		shell->pwd = NULL;
	}
	else
	{
		free(shell->oldpwd);
		shell->oldpwd = NULL;
	}
	if (getcwd(cwd, sizeof(cwd)))
		shell->pwd = ft_strdup(cwd);
	else
	{
		ft_putstr_fd("cd: warning: getcwd failed\n", STDERR_FILENO);
		shell->pwd = ft_strdup("");
	}
}

static void	update_env_pwd_vars(t_shell *shell)
{
	if (get_env_var(shell->env, "OLDPWD") && shell->oldpwd)
		update_env_field(shell->env, "OLDPWD", shell->oldpwd);
	if (get_env_var(shell->env, "PWD") && shell->pwd)
		update_env_field(shell->env, "PWD", shell->pwd);
}

int	change_directory(const char *arg, t_shell *shell)
{
	char	*path;

	path = get_abs_path(arg, shell);
	if (!path || chdir(path) != 0)
		return (handle_cd_error(arg, path, shell));
	update_pwd_and_oldpwd(shell);
	update_env_pwd_vars(shell);
	shell->exit_status = 0;
	free(path);
	return (1);
}

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*home;

	if (argv[1] && argv[2])
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
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
			return (1);
		return (0);
	}
	if (change_directory(argv[1], shell))
		return (0);
	return (1);
}
