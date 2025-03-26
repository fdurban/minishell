/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:00:47 by fdurban-          #+#    #+#             */
/*   Updated: 2024/10/01 13:11:10 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*real_dest;
	size_t				i;
	const unsigned char	*real_src;

	if (!dest && !src)
		return (NULL);
	real_dest = (unsigned char *)dest;
	real_src = (const unsigned char *)src;
	if (real_dest < real_src)
	{
		while (n--)
		{
			*real_dest++ = *real_src++;
		}
	}
	else
	{
		i = n;
		while (i > 0)
		{
			i--;
			real_dest[i] = real_src[i];
		}
	}
	return (dest);
}
