/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:11:33 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/14 15:08:47 by igngonza         ###   ########.fr       */
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
	if (!input && g_signal_state != 3)
		printf("exit\n");
	else if (input && *input && g_signal_state != 3)
		add_history(input);
	return (input);
}

void	process_command_line(char *input, t_shell *shell)
{
	t_command_part	**tok;

	tok = tokenize(input, shell);
	if (!tok)
		return ;
	shell->exit_status = execution(tok, shell);
	free_token_matrix(tok);
}

int	should_continue_after_input(char *input, t_shell *shell)
{
	if (g_signal_state == SIGINT)
	{
		shell->exit_status = 130;
		g_signal_state = 0;
		if (!input)
			return (1);
	}
	if (!input)
		return (0);
	if (*input == '\0')
	{
		free(input);
		return (1);
	}
	return (-1);
}
