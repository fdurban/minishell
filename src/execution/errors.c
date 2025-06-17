/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:35:48 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/17 13:25:51 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <sys/stat.h>

void	handle_error(const char *message)
{
	perror(message);
}

void	print_error_and_exit(char *cmd, char *msg, int code)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	exit(code);
}

void	print_exec_error_and_exit(char *cmd)
{
	struct stat	st;
	int			has_slash;

	has_slash = ft_strchr(cmd, '/') != NULL;
	if (!has_slash)
		print_error_and_exit(cmd, "command not found", 127);
	if (lstat(cmd, &st) == -1)
	{
		if (errno == ENOENT)
			print_error_and_exit(cmd, "No such file or directory", 127);
		else if (errno == EACCES)
			print_error_and_exit(cmd, "Permission denied", 126);
		else
			print_error_and_exit(cmd, strerror(errno), 126);
	}
	else if (S_ISDIR(st.st_mode))
	{
		print_error_and_exit(cmd, "is a directory", 126);
	}
	else if (!S_ISREG(st.st_mode))
	{
		print_error_and_exit(cmd, "command not found", 127);
	}
	else if (access(cmd, X_OK) == -1)
	{
		if (errno == EACCES)
			print_error_and_exit(cmd, "Permission denied", 126);
		else
			print_error_and_exit(cmd, strerror(errno), 126);
	}
	else
	{
		print_error_and_exit(cmd, "execution failed", 126);
	}
}

void	handle_redirection_error(char *file)
{
	if (errno == EACCES)
	{
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": Permission denied\n", 2);
	}
	else
	{
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
	}
}
