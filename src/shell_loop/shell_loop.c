/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:20:08 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/11 11:04:25 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_hostname(t_env *env)
{
	char	*sessionmanager;
	char	*start;
	char	*end;
	char	*hostname;

	sessionmanager = get_env_var(env, "SESSION_MANAGER");
	if (!sessionmanager)
		return (ft_strdup(""));
	start = ft_strchr(sessionmanager, '/');
	end = ft_strchr(sessionmanager, ':');
	if (!start || !end || end <= start)
		return (ft_strdup(""));
	hostname = ft_substr(start + 1, 0, (end - start) - 1);
	return (hostname);
}

char	*get_current_pwd(t_env *env)
{
	char	*home;
	char	*pwd;
	char	*currentpwd;

	home = get_env_var(env, "HOME");
	pwd = get_env_var(env, "PWD");
	currentpwd = NULL;
	if (home && pwd)
		currentpwd = ft_strjoin("~", pwd + ft_strlen(home));
	return (currentpwd);
}

char	*build_user_prompt(t_env *env, char *hostname, char *currentpwd)
{
	char	*username;
	char	*tmp;
	char	*tmp2;
	char	*prompt;

	username = get_env_var(env, "USER");
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
		input = get_user_input(shell->env);
		if (!input)
			break ;
		if (*input == '\0')
		{
			free(input);
			continue ;
		}
		process_command_line(input, shell);
		free(input);
	}
}
