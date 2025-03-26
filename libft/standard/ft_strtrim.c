/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:51:07 by fdurban-          #+#    #+#             */
/*   Updated: 2024/10/01 13:16:37 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trimmed;
	int		length;
	int		start;
	int		end;

	start = 0;
	end = ft_strlen(s1) -1;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	while (end > start && ft_strchr(set, s1[end]))
		end--;
	length = end - start + 1;
	trimmed = malloc(length +1);
	if (trimmed == NULL)
	{
		return (NULL);
	}
	ft_strlcpy(trimmed, s1 + start, length + 1);
	return (trimmed);
}
