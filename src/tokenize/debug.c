/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:48:26 by fernando          #+#    #+#             */
/*   Updated: 2025/06/16 15:03:52 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/tokenizer.h"

void	checkposition(t_word_type word_type, char *valid_command, int i)
{
	//--------------------------------------------------------------------
	printf("Letra %c\n", valid_command[i]);
	if (word_type == W___END)
		printf("word_type END and i value is : %d\n", i);
	if (word_type == W_ERROR)
		printf("word_type ERROR and i value is : %d\n", i);
	if (word_type == W_START)
		printf("word_type START and i value is : %d\n", i);
	if (word_type == W_STNDR)
		printf("word_type WORD and i value is : %d\n", i);
	if (word_type == W_SINGQ)
		printf("word_type SINGLE QUOTE and i value is : %d\n", i);
	if (word_type == W_DOUBQ)
		printf("word_type DOUBLE and i value is : %d\n", i);
	if (word_type == W_REDIN)
		printf("word_type REDIN and i value is : %d\n", i);
	if (word_type == W_REDOU)
		printf("word_type REDOU and i value is : %d\n", i);
	if (word_type == W_REDAP)
		printf("word_type REDAP and i value is : %d\n", i);
	if (word_type == W_HRDOC)
		printf("word_type HRDOC (valor %d) and i value is : %d\n", word_type,i);
	if (word_type == W_SPACE)
		printf("word_type SPACE AFTER WORD and i value is : %d\n", i);
	if (word_type == W_SARED)
		printf("word_type SPACE AFTER REDIRECT and i value is : %d\n", i);
	if (word_type == W_EOFSQ)
		printf("word_type END OF SINGLE QUOTE and i value is : %d\n", i);
	if (word_type == W_EOFDQ)
		printf("word_type END OF DOUBLE QUOTE and i value is : %d\n", i);
	if (word_type == W_EOFST)
		printf("word_type END OF STANDARD and i value is : %d\n", i);
	if (word_type == W_EOSTS)
		printf("word_type END OF STANDARD TO SINGLE QUOTE and i value is :%d\n",i);
	if (word_type == W_EOSTD)
		printf("word_type END OF STANDARD TO DOUBLE QUOTE and i value is :%d\n",i);
	//----------------------------------------------------------------------------
}

void	checkinput(t_input_tokenizer input)
{
	if (input == I_SPACE)
		printf("INPUT SPACE\n");
	if (input == I_IN_SINGLE_QUOTE)
		printf("INPUT TOKEN_IN_SINGLE_QUOTE\n");
	if (input == I_IN_DOUBLE_QUOTE)
		printf("INPUT TOKEN_IN_DOUBLE_QUOTE\n");
	if (input == I_LETTER)
		printf("INPUT_TOKEN_LETTER\n");
	if (input == I_REDIRECT_IN)
		printf("INPUT_TOKEN_REDIRECT_IN\n");
	if (input == I_REDIRECT_OUT)
		printf("INPUT_TOKEN_REDIRECT_OUT\n");
	if (input == I_END)
		printf("INPUT_TOKEN_END\n");
}

void	print_values(t_command_part **results)
{
	printf("Commands:\n");
	for (int j = 0; results[j]; j++)
	{
		t_command_part *tmp = results[j];
		printf("Comando #%d:\n", j);
		if (!results[j] || results)
		{
			printf("Nothing to print\n");
			return ;
		}
		while (tmp)
		{
			printf("  Valor: %s, Tipo: %d\n", tmp->value, tmp->type);
			tmp = tmp->next;
		}
	}
}