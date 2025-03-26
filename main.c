#include <stdio.h>

int main(int argc, char **argv, char **envp)
{
	printf("Number of arguments is %d\n", argc);
	printf("%s\n", argv[0]);
	while(*envp)
	{
		printf("environment variable: %s\n", *envp);
		envp++;
	}
}