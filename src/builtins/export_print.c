/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:52:03 by igngonza          #+#    #+#             */
/*   Updated: 2025/05/12 18:09:36 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	my_strcmp(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	size_t	cmp_len;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if (len1 > len2)
		cmp_len = len1 + 1;
	else
		cmp_len = len2 + 1;
	return (ft_strncmp(s1, s2, cmp_len));
}

static void	sort_array(char **arr, int count)
{
	int		swapped;
	int		i;
	char	*tmp;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (i < count - 1)
		{
			if (my_strcmp(arr[i], arr[i + 1]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
	}
}

static char	**get_sorted_env(t_env *env)
{
	int		i;
	char	**sorted;

	sorted = malloc(sizeof(char *) * (env->count + 1));
	if (!sorted)
		return (NULL);
	i = 0;
	while (i < env->count)
	{
		sorted[i] = env->vars[i];
		i++;
	}
	sorted[i] = NULL;
	sort_array(sorted, env->count);
	return (sorted);
}

static void	print_single_exported_env(char *var)
{
	char	*eq;

	eq = ft_strchr(var, '=');
	ft_putstr_fd("declare -x ", 1);
	if (eq)
	{
		write(1, var, (eq - var));
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(eq + 1, 1);
		ft_putstr_fd("\"\n", 1);
	}
	else
	{
		ft_putstr_fd(var, 1);
		write(1, "\n", 1);
	}
}

void	print_exported_env(t_env *env)
{
	int i;
	char **sorted;

	sorted = get_sorted_env(env);
	if (sorted == NULL)
		return ;
	i = 0;
	while (i < env->count)
	{
		print_single_exported_env(sorted[i]);
		i++;
	}
	free(sorted);
}