/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:14:09 by igngonza          #+#    #+#             */
/*   Updated: 2025/05/13 16:36:28 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_command_line(char *input, t_shell *shell);
char	*get_user_input(t_env *env);
char	*get_hostname(t_env *env);
char	*get_current_pwd(t_env *env);
char	*build_user_prompt(t_env *env, char *hostname, char *currentpwd);
char	*build_prompt(t_env *env);
void	shell_loop(t_shell *shell);
