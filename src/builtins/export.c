/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:26:24 by igngonza          #+#    #+#             */
/*   Updated: 2025/05/12 18:09:21 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_identifier(const char *s)
{
	size_t	i;

	if (!s || !s[0] || (!(ft_isalpha(s[0]) || s[0] == '_')))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void	process_export_with_value(char *arg, t_shell *shell)
{
	char	*eq;
	char	*key;
	char	*value;

	eq = ft_strchr(arg, '=');
	key = ft_substr(arg, 0, eq - arg);
	value = ft_strdup(eq + 1);
	update_env_field(shell->env, key, value);
	free(key);
	free(value);
}

static void	process_export_without_value(char *arg, t_shell *shell)
{
	char	*new_var;

	if (!get_env_var(shell->env, arg))
	{
		new_var = ft_strdup(arg);
		if (!new_var)
			return ;
		append_env_field(shell->env, new_var);
	}
}

int	builtin_export(char **args, t_shell *shell)
{
	int i;

	if (!args[1])
	{
		print_exported_env(shell->env);
		return (0);
	}
	shell->exit_status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			shell->exit_status = 1;
		}
		else if (ft_strchr(args[i], '='))
			process_export_with_value(args[i], shell);
		else
			process_export_without_value(args[i], shell);
		i++;
	}
	return (shell->exit_status);
}