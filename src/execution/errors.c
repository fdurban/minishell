/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:35:48 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/09 11:56:59 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_error(const char *message)
{
	perror(message);
}

void	handle_pipe_msg(char *arg)
{
	write(2, ERR_CMD, ft_strlen(ERR_CMD));
	write(2, arg, ft_strlen(arg));
	write(2, "\n", 1);
}

int	handle_msg(char *err)
{
	write(2, err, ft_strlen(err));
	return (1);
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
	if (errno == ENOENT)
		print_error_and_exit(cmd, "No such file or directory", 127);
	else if (errno == EACCES)
		print_error_and_exit(cmd, "Permission denied", 126);
	else if (errno == EISDIR)
		print_error_and_exit(cmd, "is a directory", 126);
	else
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		exit(126);
	}
}
