/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:45:03 by fdurban-          #+#    #+#             */
/*   Updated: 2025/07/19 12:36:53 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/tokenizer.h"

char	*append_prefix(char *result, char *word_value, int start, int end)
{
	char	*prefix;
	char	*tmp;

	prefix = ft_substr(word_value, start, end - start);
	tmp = ft_strjoin(result, prefix);
	free(prefix);
	free(result);
	return (tmp);
}

static int	get_var_name_length(const char *s, int *start)
{
	int	length;

	length = 0;
	if (s[*start] == '?' || s[*start] == '-' || s[*start] == '$')
		return (1);
	while (s[*start + length] != '\0' && s[*start + length] != ' ')
	{
		length++;
	}
	return (length);
}

static char	*get_var_value(const char *var_name, t_shell *shell)
{
	char	*env_val;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(shell->exit_status));
	if (ft_strcmp(var_name, "PWD") == 0)
	{
		env_val = get_env_var(shell->env, "PWD");
		if (!env_val && shell->pwd)
			return (ft_strdup(shell->pwd));
	}
	else if (ft_strcmp(var_name, "OLDPWD") == 0)
	{
		env_val = get_env_var(shell->env, "OLDPWD");
		if (!env_val && shell->oldpwd)
			return (ft_strdup(shell->oldpwd));
	}
	else
		env_val = get_env_var(shell->env, var_name);
	if (env_val)
		return (ft_strdup(env_val));
	return (ft_strdup(""));
}

char	*append_variable(char *result, int *i, char *word, t_shell *shell)
{
	int		start;
	int		len;
	char	*new_result;
	char	*var_name;
	char	*value;

	start = *i + 1;
	len = get_var_name_length(word, &start);
	if (word[*i + 1] == '\0' || word[*i + 1] == ' ' || word[*i + 1] == '/'
		|| word[*i + 1] == '+' || word[*i + 1] == '\'')
	{
		new_result = ft_strjoin(result, "$");
		free(result);
		*i += 1;
		return (new_result);
	}
	var_name = ft_substr(word, start, len);
	value = get_var_value(var_name, shell);
	new_result = ft_strjoin(result, value);
	free(result);
	free(var_name);
	free(value);
	*i = start + len;
	return (new_result);
}

char	*expand_token(char *word, t_shell *shell)
{
	char	*result;
	int		i;
	int		start;

	i = 0;
	result = ft_strdup("");
	while (word[i] != '\0')
	{
		start = i;
		while (word[i] && word[i] != '$')
			i++;
		result = append_prefix(result, word, start, i);
		if (word[i] == '$')
			result = append_variable(result, &i, word, shell);
		if (!result)
			result = ft_strjoin(result, "\n");
	}
	return (result);
}
