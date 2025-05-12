/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:15:19 by igngonza          #+#    #+#             */
/*   Updated: 2025/05/12 18:32:20 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*append_env_field(t_env *env, char *new_var)
{
	char	**temp;

	temp = realloc(env->vars, sizeof(char *) * (env->count + 2));
	if (!temp)
	{
		free(new_var);
		return (NULL);
	}
	env->vars = temp;
	env->vars[env->count] = new_var;
	env->count++;
	env->vars[env->count] = NULL;
	return (new_var);
}

static char	*create_env_string(const char *key, const char *value)
{
	size_t	total_len;
	char	*new_var;
	size_t	key_len;
	size_t	i;
	size_t	j;

	key_len = ft_strlen(key);
	total_len = key_len + ft_strlen(value) + 2;
	i = 0;
	j = 0;
	new_var = malloc(total_len);
	if (!new_var)
		return (NULL);
	while (i < key_len)
	{
		new_var[i] = key[i];
		i++;
	}
	new_var[i++] = '=';
	while (value[j])
		new_var[i++] = value[j++];
	new_var[i] = '\0';
	return (new_var);
}

static int	replace_existing_env(t_env *env, const char *key, char *new_var)
{
	size_t	key_len;
	int		i;

	key_len = ft_strlen(key);
	i = 0;
	while (i < env->count)
	{
		if (ft_strncmp(env->vars[i], key, key_len) == 0
			&& env->vars[i][key_len] == '=')
		{
			free(env->vars[i]);
			env->vars[i] = new_var;
			return (1);
		}
		i++;
	}
	return (0);
}

char	*update_env_field(t_env *env, const char *key, const char *value)
{
	char	*new_var;

	if (!env || !key || !value)
		return (NULL);
	new_var = create_env_string(key, value);
	if (!new_var)
		return (NULL);
	if (get_env_var(env, key) != NULL)
	{
		if (replace_existing_env(env, key, new_var))
			return (new_var);
	}
	return (append_env_field(env, new_var));
}
