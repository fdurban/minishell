/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:11:33 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/15 16:54:26 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_current_pwd(t_env *env)
{
	char	*home;
	char	*pwd;
	char	*currentpwd;
	char	*cwd;

	home = get_env_var(env, "HOME");
	pwd = get_env_var(env, "PWD");
	if (!pwd)
	{
		cwd = getcwd(NULL, 0);
		if (home && ft_strncmp(home, cwd, ft_strlen(home)) == 0)
			currentpwd = ft_strjoin("~", cwd + ft_strlen(home));
		else
			currentpwd = ft_strdup(cwd);
		free(cwd);
		return (currentpwd);
	}
	if (home && ft_strncmp(home, pwd, ft_strlen(home)) == 0)
		currentpwd = ft_strjoin("~", pwd + ft_strlen(home));
	else
		currentpwd = ft_strdup(pwd);
	return (currentpwd);
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

char	*get_user_input(t_env *env)
{
	char	*prompt;
	char	*input;

	prompt = build_prompt(env);
	input = readline(prompt);
	free(prompt);
	if (!input && g_signal_state != 3)
		printf("exit\n");
	else if (input && *input && g_signal_state != 3)
		add_history(input);
	return (input);
}

void	process_command_line(char *input, t_shell *shell)
{
	t_command_part	**tok;

	tok = tokenize(input, shell);
	if (!tok)
		return ;
	shell->exit_status = execution(tok, shell);
	free_token_matrix(tok);
}
