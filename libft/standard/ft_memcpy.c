/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:59:11 by fdurban-          #+#    #+#             */
/*   Updated: 2024/10/01 13:11:01 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*real_dest;
	unsigned char	*real_src;
	size_t			i;

	if (!dest && !src)
		return (NULL);
	real_dest = (unsigned char *)dest;
	real_src = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		real_dest[i] = real_src[i];
		i++;
	}
	return (dest);
}
