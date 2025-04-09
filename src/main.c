#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_copy;

	(void)argc;
	(void)argv;
	env_copy = copy_env(envp);
	if (!env_copy)
	{
		fprintf(stderr, "Failed to copy environment\n");
		return (1);
	}
	shell_loop(env_copy);
	free_env(env_copy);
	return (0);
}
