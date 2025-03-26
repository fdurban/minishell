/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:46:25 by fdurban-          #+#    #+#             */
/*   Updated: 2024/10/01 13:16:21 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h" 

char	*ft_strnstr(char *str, char *to_find, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (*to_find == '\0')
	{
		return (str);
	}
	while (str[i] != '\0' && i < len)
	{
		j = 0;
		while (to_find[j] != '\0' && str[i + j] == to_find[j] && (i + j) < len)
		{
			j++;
		}
		if (to_find[j] == '\0')
		{
			return ((char *)&str[i]);
		}
		i++;
	}
	return (0);
}
