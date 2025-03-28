#include "../minishell.h"

int main(int argc, char **argv)
{
	char *input;
	char	*username;
	char	*sessionmanager;
	char	*hostname;
	char	*home;
	char	*currentpwd;
	char	**tokens;


	username = getenv("USER");
	sessionmanager = getenv("SESSION_MANAGER");
	char	*start = ft_strchr(sessionmanager, '/');
	char	*end = ft_strchr(sessionmanager, '.');
	hostname = ft_substr(start + 1, 0, (end - start) - 1);
	home = getenv("HOME");
	currentpwd = ft_strjoin("~", getenv("PWD") + ft_strlen(home));
	printf("%d\n", argc);
	printf("%s\n", argv[0]);

	while (1)
	{
		input = readline(ft_strjoin(username, ft_strjoin("@", ft_strjoin(hostname, currentpwd))));
		tokens = ft_split(input, '|');
		printf("%s\n", tokens[0]);
		if (*input)
			add_history(input);
		free(input);
	}
	return 0;
}
