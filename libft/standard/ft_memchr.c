/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 14:14:17 by fdurban-          #+#    #+#             */
/*   Updated: 2024/10/01 13:10:38 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*byte_s;
	unsigned char		byte_c ;
	size_t				i;

	i = 0;
	byte_s = (const unsigned char *)s;
	byte_c = (unsigned char)c;
	while (i < n)
	{
		if (byte_s[i] == byte_c)
		{
			return ((void *)(byte_s + i));
		}
		i++;
	}
	return (NULL);
}
