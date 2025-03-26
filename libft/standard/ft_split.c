/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:51:58 by fdurban-          #+#    #+#             */
/*   Updated: 2025/02/24 16:42:56 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	wordcount(char const *s, char c)
{
	int		i;
	int		phrasecount;
	int		inword;

	i = 0;
	inword = 0;
	phrasecount = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && !inword)
		{
			inword = 1;
			phrasecount++;
		}
		else if (s[i] == c)
			inword = 0;
		i++;
	}
	return (phrasecount);
}

static	void	free_memory(char	**main_mem, int i)
{
	while (i >= 0)
	{
		free(main_mem[i]);
		i--;
	}
	free(main_mem);
}

static	char	*ft_putword(char const *s, char c)
{
	int		i;
	int		len;
	char	*str;

	i = 0;
	len = 0;
	while (s[len] && s[len] != c)
	{
		len++;
	}
	str = malloc((len + 1) * sizeof(char));
	if (str == NULL)
	{
		return (NULL);
	}
	while (i < len)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static	char	*ft_save(char const *s, int i, char c, char **main_mem)
{
	main_mem[i] = ft_putword(s, c);
	if (main_mem[i] == NULL)
	{
		free_memory(main_mem, i);
		return (NULL);
	}
	return (main_mem[i]);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		count;
	char	**main_mem;
	int		j;

	j = 0;
	i = 0;
	count = wordcount(s, c);
	main_mem = malloc((sizeof(char *) * (count + 1)));
	if (!main_mem)
		return (NULL);
	while (i < count)
	{
		while (s[j] == c)
			j++;
		main_mem[i] = ft_save(&s[j], i, c, main_mem);
		if (!main_mem[i])
			return (NULL);
		while (s[j] != c && s[j] != '\0')
			j++;
		i++;
	}
	main_mem[count] = NULL;
	return (main_mem);
}
