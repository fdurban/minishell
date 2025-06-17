/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:45:03 by fdurban-          #+#    #+#             */
/*   Updated: 2025/06/17 14:41:16 by igngonza         ###   ########.fr       */
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

static int	get_var_name_length(const char *s, int start)
{
	int	length;

	length = 0;
	if (s[start] == '?')
		return (1);
	while (ft_isalnum(s[start + length]) || s[start + length] == '_')
		length++;
	return (length);
}

static char	*extract_var_name(const char *s, int start, int len)
{
	return (ft_substr(s, start, len));
}

static char	*get_var_value(const char *var_name, t_shell *shell)
{
	char	*env_val;
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(shell->exit_status));
	env_val = get_env_var(shell->env, var_name);
	if (env_val != NULL)
		value = ft_strdup(env_val);
	else
		value = ft_strdup("");
	return (value);
}

char	*append_variable(char *result, int *i, t_command_part *word,
		t_shell *shell)
{
	int		start;
	int		len;
	char	*new_result;
	char	*var_name;
	char	*value;

	start = *i + 1;
	len = get_var_name_length(word->value, start);
	if (len == 0)
	{
		new_result = ft_strjoin(result, "$");
		free(result);
		*i = start;
		return (new_result);
	}
	{
		var_name = extract_var_name(word->value, start, len);
		value = get_var_value(var_name, shell);
		new_result = ft_strjoin(result, value);
		free(result);
		free(var_name);
		free(value);
		*i = start + len;
		return (new_result);
	}
}

char	*expand_token(t_command_part *word, t_shell *shell)
{
	char	*result;
	int		i;
	int		start;
			char *temp_result;

	i = 0;
	result = ft_strdup("");
	while (word->value[i] != '\0')
	{
		start = i;
		while (word->value[i] && word->value[i] != '$')
			i++;
		result = append_prefix(result, word->value, start, i);
		if (word->value[i] == '$')
		{
			temp_result = append_variable(result, &i, word, shell);
			if (temp_result == NULL)
			{
				free(result);
				result = ft_strdup("");
			}
			else
				result = temp_result;
		}
	}
	return (result);
}
