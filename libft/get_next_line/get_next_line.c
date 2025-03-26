/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:38:22 by fdurban-          #+#    #+#             */
/*   Updated: 2025/03/04 18:02:16 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strchr_gnl(char *string, int c)
{
	int		i;
	char	n;

	n = (char)c;
	i = 0;
	if (c == '\0')
		return (&string[ft_strlen(string)]);
	while (string[i] != '\0')
	{
		if (string[i] == n)
		{
			return (&string[i]);
		}
		i++;
	}
	return (NULL);
}

char	*ft_str_result(char *s)
{
	size_t	i;
	char	*save;

	i = 0;
	if (!s[i] || !s)
		return (NULL);
	while (s[i] && s[i] != '\n')
		i++;
	save = malloc(sizeof(char) * (i + 2));
	if (!save)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != '\n')
	{
		save[i] = s[i];
		i++;
	}
	if (s[i] == '\n')
	{
		save[i] = s[i];
		i++;
	}
	save[i] = '\0';
	return (save);
}

char	*ft_remove_line(char	*str)
{
	size_t	i;
	size_t	j;
	char	*save;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
	{
		free(str);
		return (NULL);
	}
	save = malloc(sizeof(char) * (ft_strlen(str) - i + 1));
	if (!str || !save)
		return (NULL);
	i++;
	while (str[i])
		save[j++] = str[i++];
	save[j] = '\0';
	free(str);
	return (save);
}

char	*get_until_jump(int fd, char *long_mem)
{
	long			bytes_read;
	char			*mem;

	bytes_read = 1;
	mem = malloc((BUFFER_SIZE + 1) * sizeof(char));
	while (bytes_read > 0)
	{
		bytes_read = read(fd, mem, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(mem);
			free(long_mem);
			return (NULL);
		}
		mem[bytes_read] = '\0';
		long_mem = ft_strjoin_gnl(long_mem, mem);
		if (ft_strchr_gnl(long_mem, '\n'))
			break ;
	}
	free(mem);
	return (long_mem);
}

char	*get_next_line(int fd)
{
	char			*result;
	static char		*long_mem;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		if (long_mem)
		{
			free(long_mem);
			long_mem = NULL;
		}
		return (NULL);
	}
	long_mem = get_until_jump(fd, long_mem);
	if (!long_mem)
		return (NULL);
	result = ft_str_result(long_mem);
	long_mem = ft_remove_line(long_mem);
	return (result);
}
