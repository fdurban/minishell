/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:31:10 by igngonza          #+#    #+#             */
/*   Updated: 2025/05/12 18:09:03 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_env(char **args, t_shell *shell)
{
	int i;

	(void)args;
	i = 0;
	while (i < shell->env->count)
	{
		if (ft_strchr(shell->env->vars[i], '='))
			printf("%s\n", shell->env->vars[i]);
		i++;
	}
	return (0);
}