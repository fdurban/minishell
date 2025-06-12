/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:11:33 by igngonza          #+#    #+#             */
/*   Updated: 2025/06/12 12:02:09 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static char	**build_pipex_tokens(t_command_part **tokarr)
{
	int		cmd_cnt;
	char	**argv;
	char	**tokens;
	int		i;
	int		j;
	int		k;
	int		total;

	cmd_cnt = 0;
	total = 0;
	i = 0;
	j = 0;
	k = 0;
	// 1) count commands
	while (tokarr[cmd_cnt])
		cmd_cnt++;
	// 2) count total words + (cmd_cnt-1) pipe tokens
	for (i = 0; i < cmd_cnt; i++)
	{
		argv = tokens_to_argv(tokarr[i]);
		for (j = 0; argv[j]; j++)
			total++;
		free_argv(argv);
	}
	total += (cmd_cnt - 1);
	// 3) allocate and fill
	tokens = malloc(sizeof(char *) * (total + 1));
	k = 0;
	for (i = 0; i < cmd_cnt; i++)
	{
		argv = tokens_to_argv(tokarr[i]);
		for (j = 0; argv[j]; j++)
			tokens[k++] = strdup(argv[j]);
		free_argv(argv);
		if (i + 1 < cmd_cnt)
			tokens[k++] = strdup("|");
	}
	tokens[k] = NULL;
	return (tokens);
}

static void	free_pipex_tokens(char **tokens)
{
	for (int i = 0; tokens[i]; i++)
		free(tokens[i]);
	free(tokens);
}

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
	int				i;

	segs = 0;
	tok = tokenize(input, shell);
	if (!tok[0])
	{
		free(tok);
		return ;
	}
	while (tok[segs])
		segs++;
	if (segs == 1)
	{
		argv = tokens_to_argv(tok[0]);
		if (argv && argv[0] && argv[0][0] == '\0')
		{
			shell->exit_status = 0;
			i = 0;
			while (argv[i])
			{
				free(argv[i]);
				i++;
			}
			free(argv);
			free(tok);
			return ;
		}
		if (argv && argv[0])
		{
			if (strcmp(argv[0], "cd") == 0 || strcmp(argv[0], "export") == 0
				|| strcmp(argv[0], "unset") == 0 || strcmp(argv[0],
					"exit") == 0)
			{
				shell->exit_status = exec_builtin(argv, shell);
				for (int i = 0; argv[i]; i++)
					free(argv[i]);
				free(argv);
				free(tok);
				return ;
			}
		}
		for (int i = 0; argv && argv[i]; i++)
			free(argv[i]);
		free(argv);
	}
	flat = build_pipex_tokens(tok);
	shell->exit_status = execution(flat, shell);
	free_pipex_tokens(flat);
	free(tok);
}
