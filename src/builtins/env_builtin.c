/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:31:10 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/19 13:56:26 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_env(char **args, t_shell *shell)
{
	int	i;

	(void)args;
	i = 0;
	while (i < shell->env->count)
	{
		if (ft_strchr(shell->env->vars[i], '='))
			ft_printf("%s\n", shell->env->vars[i]);
		i++;
	}
	return (0);
}
