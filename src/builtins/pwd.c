/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:42:16 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/14 13:17:41 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_current_directory(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (NULL);
	}
	return (cwd);
}

void	print_directory(const char *directory)
{
	int	i;
	int	length;

	i = 0;
	length = ft_strlen(directory);
	while (i < length)
	{
		write(1, &directory[i], 1);
		i++;
	}
	write(1, "\n", 1);
}

int	builtin_pwd(char **args, t_shell *shell)
{
	char	*pwd;
	char	*allocated_pwd;

	(void)args;
	pwd = get_env_var(shell->env, "PWD");
	allocated_pwd = NULL;
	if (!pwd)
	{
		allocated_pwd = get_current_directory();
		if (!allocated_pwd)
			return (1);
		pwd = allocated_pwd;
	}
	print_directory(pwd);
	if (allocated_pwd)
		free(allocated_pwd);
	return (0);
}
