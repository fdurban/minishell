/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:11:33 by igngonza          #+#    #+#             */
/*   Updated: 2025/05/13 15:55:49 by igngonza         ###   ########.fr       */
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
	t_command_part **tokens_array;
	t_command_part *command_tokens;
	char **argv;

	tokens_array = tokenize(input);
	command_tokens = tokens_array[0];
	if (command_tokens)
	{
		argv = tokens_to_argv(command_tokens);
		if (argv && argv[0])
		{
			if (is_builtin(argv[0]))
				exec_builtin(argv, shell);
			else
				; // Later: call function to handle external execution (execve)
		}
		for (int i = 0; argv && argv[i]; i++)
			free(argv[i]);
		free(argv);
	}
	free(tokens_array);
}