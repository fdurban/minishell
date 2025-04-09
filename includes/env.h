/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:10:35 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/09 20:10:36 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <limits.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_env
{
	char	**vars;
	int		count;
}			t_env;

t_env		*copy_env(char **envp);
char		*get_env_var(t_env *env, const char *key);
void		free_env(t_env *env);

#endif