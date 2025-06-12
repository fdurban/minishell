/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:11:33 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/12 12:32:06 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_user_input(t_env *env)
{
	char	*prompt;
	char	*input;

	prompt = build_prompt(env);
	input = readline(prompt);
	free(prompt);
	if (input && *input)
		add_history(input);
	return (input);
}

void	process_command_line(char *input, t_shell *shell)
{
	t_command_part	**tok;
	char			**flat;
	char			**argv;
	int				segs;

	tok = tokenize(input, shell->env);
	if (!tok[0])
	{
		free(tok);
		return ;
	}
	shell->exit_status = execution(tok, shell);
	free(tok);
}
