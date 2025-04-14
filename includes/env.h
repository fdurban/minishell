/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:10:35 by igngonza          #+#    #+#             */
/*   Updated: 2025/04/11 13:17:14 by igngonza         ###   ########.fr       */
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

char		*append_env_field(t_env *env, char *new_var);
char		*update_env_field(t_env *env, const char *key, const char *value);
void		free_env(t_env *env);

#endif