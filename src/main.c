#include "../minishell.h"

int main(int argc, char **argv, char **env)
{
	char *input;
	char	*username;
	char	*sessionmanager;
	char	*hostname;
	char	*home;
	char	*currentpwd;


	username = getenv("USER");
	sessionmanager = getenv("SESSION_MANAGER");
	char	*start = ft_strchr(sessionmanager, '/');
	char	*end = ft_strchr(sessionmanager, '.');
	hostname = ft_substr(start + 1, 0, (end - start) - 1);
	home = getenv("HOME");
	currentpwd = ft_strjoin("~", getenv("PWD") + ft_strlen(home));
	printf("%d\n", argc);
	printf("%s\n", argv[0]);
	char *arguments[] = {"ls", "-la", NULL};

	while (1)
	{
		input = readline(ft_strjoin(username, ft_strjoin("@", ft_strjoin(hostname, currentpwd))));
		if (*input) add_history(input);

		if (strcmp(input, "exit") == 0)
		{
			free(input);
			break;
		}
		if (strcmp(input, "ls") == 0)
		{
			execve("/bin/ls", arguments, env);
		}
		if (strcmp(input, "clear") == 0)
		{
			rl_clear_history();
		}
		free(input);
	}
	return 0;
}
