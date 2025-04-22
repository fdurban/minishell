void	print_tokens_by_space(char ***parsed_tokens, int token_number)
{
	int i = 0;
	while (i < token_number)
	{
		int j = 0;
		while(parsed_tokens[i][j])
		{
			printf("[%s]\n", parsed_tokens[i][j]);
			j++;
		}
		i++;
	}
}