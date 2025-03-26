/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:22:50 by fdurban-          #+#    #+#             */
/*   Updated: 2024/10/01 13:16:31 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(char *string, int c)
{
	int		i;
	char	n;

	i = 0;
	n = (char) c;
	while (string[i] != '\0')
	{
		i++;
	}
	while (i >= 0)
	{
		if (string[i] == n)
		{
			return (&string[i]);
		}
		i--;
	}
	if (n == '\0')
	{
		return (&string[i]);
	}
	return (NULL);
}
