/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:01:34 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/15 15:17:17 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	matches_key(const char *entry, const char *key)
{
	int	len;

	len = ft_strlen(key);
	if (ft_strncmp(entry, key, len) == 0 && (entry[len] == '='
			|| entry[len] == '\0'))
		return (1);
	return (0);
}

int	builtin_unset(char **args, t_shell *shell)
{
	int i;
	int j;

	i = 1;
	while (args[i])
	{
		j = 0;
		while (j < shell->env->count)
		{
			if (matches_key(shell->env->vars[j], args[i]))
			{
				free(shell->env->vars[j]);
				while (j < shell->env->count - 1)
				{
					shell->env->vars[j] = shell->env->vars[j + 1];
					j++;
				}
				shell->env->count = shell->env->count - 1;
				shell->env->vars[shell->env->count] = NULL;
				break ;
			}
			j++;
		}
		i++;
	}
	return (0);
}