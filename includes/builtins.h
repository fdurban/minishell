/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:04:03 by igngonza          #+#    #+#             */
/*   Updated: 2025/05/12 18:04:09 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "env.h"
# include <stdio.h>
# include <string.h>

typedef struct s_shell
{
	t_env			*env;
	int				exit_status;
}					t_shell;

typedef int			(*t_builtin_fn)(char **args, t_shell *shell);

typedef struct s_builtin
{
	char			*name;
	t_builtin_fn	func;
}					t_builtin;

int					exec_builtin(char **args, t_shell *shell);
int					is_builtin(const char *cmd);

int					builtin_echo(char **args, t_shell *shell);
int					builtin_cd(char **argv, t_shell *shell);
char				*handle_dot_paths(const char *arg, t_shell *shell);
int					builtin_pwd(char **args, t_shell *shell);
int					builtin_export(char **args, t_shell *shell);
void				print_exported_env(t_env *env);
int					builtin_unset(char **args, t_shell *shell);
int					builtin_env(char **args, t_shell *shell);
int					builtin_exit(char **args, t_shell *shell);

#endif