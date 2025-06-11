/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernando <fernando@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:45:03 by fdurban-          #+#    #+#             */
/*   Updated: 2025/06/02 20:45:46 by fernando         ###   ########.fr       */
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

char	*append_variable(char *result, int *i, t_command_part *word, t_env *env)
{
	int		start;
	int		len;
	char	*expanded_token;
	char	*tmp;
	char	*var_name;

	start = *i + 1;
	len = 0;
	while (ft_isalnum(word->value[start + len]) || \
	word->value[start + len] == '_')
		len++ ;
	var_name = ft_substr(word->value, start, len);
	expanded_token = get_env_var(env, var_name);
	free(var_name);
	tmp = ft_strjoin(result, expanded_token);
	free(result);
	*i += len + 1;
	return (tmp);
}

char	*expand_token(t_command_part *word, t_env *env)
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
			result = append_variable(result, &i, word, env);
		if (!result)
			result = ft_strjoin(result, "\n");
	}
	return (result);
}
