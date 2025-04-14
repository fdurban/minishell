/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:33:14 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/11 13:33:29 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*copy_env(char **envp)
{
	t_env	*env;
	int		count;
	int		i;

	count = 0;
	while (envp[count])
		count++;
	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->vars = malloc(sizeof(char *) * (count + 1));
	if (!env->vars)
	{
		free(env);
		return (NULL);
	}
	i = 0;
	while (i < count)
	{
		env->vars[i] = strdup(envp[i]);
		i++;
	}
	env->vars[count] = NULL;
	env->count = count;
	return (env);
}

char	*get_env_var(t_env *env, const char *key)
{
	size_t	key_len;
	int		i;

	if (!env || !key)
		return (NULL);
	key_len = strlen(key);
	i = 0;
	while (i < env->count)
	{
		if (strncmp(env->vars[i], key, key_len) == 0
			&& env->vars[i][key_len] == '=')
			return (env->vars[i] + key_len + 1);
		i++;
	}
	return (NULL);
}
