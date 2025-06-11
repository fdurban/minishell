/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:45:03 by fdurban-          #+#    #+#             */
/*   Updated: 2025/06/11 15:30:44 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/tokenizer.h"
#include "../../includes/minishell.h"

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

char	*append_variable(char *result, int *i, t_command_part *word, t_shell *shell)
{
	int		start;
	int		len;
	char	*expanded_token;
	char	*tmp;
	char	*var_name;

	start = *i + 1;
	len = 0;
	while (word->value[start + len] && (ft_isalnum(word->value[start + len]) || \
	word->value[start + len] == '_'))
		len++ ;
	var_name = ft_substr(word->value, start, len);
	if(!var_name)
		return (NULL);
	if (ft_strncmp(var_name, "?", 2) == 0)
	{
		free(var_name);
		expanded_token = ft_itoa(shell->exit_status);
	}
	else
	{
		expanded_token = get_env_var(shell->env, var_name);
		if (!expanded_token)
			return (NULL);
		//printf("El valor de expanded_tokenant es %s\n", expanded_token);
		free(var_name);
	}
	tmp = ft_strjoin(result, expanded_token);
	free(result);
	*i += len + 1;
	return (tmp);
}

char	*expand_token(t_command_part *word, t_shell *shell)
{
	char	*result;
	int		i;
	int		start;

	i = 0;
	result = ft_strdup("");
	while (word->value[i] != '\0')
	{
		start = i;
		while (word->value[i] && word->value[i] != '$')
			i++ ;
		result = append_prefix(result, word->value, start, i);
		if (word->value[i] == '$')
		{
			result = append_variable(result, &i, word, shell);
		}
		if (!result)
		{
			result = ft_strdup("");
			return (result);
		}
	}
	return (result);
}
