/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:20:08 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/14 12:58:17 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_hostname(t_env *env)
{
	char	*sessionmanager;
	char	*start;
	char	*end;
	char	*hostname;
	char	*dot;

	sessionmanager = get_env_var(env, "SESSION_MANAGER");
	if (!sessionmanager)
		return (ft_strdup(""));
	start = ft_strchr(sessionmanager, '/');
	end = ft_strchr(sessionmanager, ':');
	if (!start || !end || end <= start)
		return (ft_strdup(""));
	hostname = ft_substr(start + 1, 0, (end - start) - 1);
	dot = ft_strchr(hostname, '.');
	if (dot)
		*dot = '\0';
	return (hostname);
}

char	*get_current_pwd(t_env *env)
{
	char	*home;
	char	*pwd;
	char	*currentpwd;

	home = get_env_var(env, "HOME");
	pwd = get_env_var(env, "PWD");
	if (!pwd)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
			perror("getcwd");
	}
	currentpwd = NULL;
	if (home && pwd && ft_strncmp(home, pwd, ft_strlen(home)) == 0)
		currentpwd = ft_strjoin("~", pwd + ft_strlen(home));
	else if (pwd)
		currentpwd = ft_strdup(pwd);
	else
		currentpwd = ft_strdup("");
	return (currentpwd);
}

char	*build_user_prompt(t_env *env, char *hostname, char *currentpwd)
{
	char	*username;
	char	*tmp;
	char	*tmp2;
	char	*prompt;

	username = get_env_var(env, "USER");
	if (!username)
		username = "";
	tmp = ft_strjoin(username, "@");
	tmp2 = ft_strjoin(tmp, hostname);
	free(tmp);
	tmp = ft_strjoin(tmp2, ":");
	free(tmp2);
	tmp2 = ft_strjoin(tmp, currentpwd);
	free(tmp);
	prompt = ft_strjoin(tmp2, "$ ");
	free(tmp2);
	return (prompt);
}

char	*build_prompt(t_env *env)
{
	char	*hostname;
	char	*currentpwd;
	char	*prompt;

	hostname = get_hostname(env);
	currentpwd = get_current_pwd(env);
	if (!hostname)
		hostname = ft_strjoin("", "");
	if (!currentpwd)
		currentpwd = ft_strjoin("", "");
	prompt = build_user_prompt(env, hostname, currentpwd);
	free(hostname);
	free(currentpwd);
	return (prompt);
}

void	shell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		shell->state = SHELL_MAIN;
		set_signal_handlers(SHELL_MAIN);
		g_signal_state = 0;
		input = get_user_input(shell->env);
		if (g_signal_state == SIGINT)
		{
			shell->exit_status = 130;
			g_signal_state = 0;
			if (!input)
				continue ;
		}
		if (!input)
			break ;
		if (*input == '\0')
		{
			free(input);
			continue ;
		}
		shell->state = SHELL_EXEC;
		set_signal_handlers(shell->state);
		process_command_line(input, shell);
		free(input);
	}
}
