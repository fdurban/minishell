/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 10:17:30 by fdurban-          #+#    #+#             */
/*   Updated: 2025/01/21 12:20:03 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *string, int c)
{
	int		i;
	char	n;
	char	*endstring;

	endstring = (char *)string;
	n = (char)c;
	i = 0;
	while (endstring[i] != '\0')
	{
		if (endstring[i] == n)
		{
			return (&endstring[i]);
		}
		i++;
	}
	if (n == '\0')
	{
		return (&endstring[i]);
	}
	return (NULL);
}
