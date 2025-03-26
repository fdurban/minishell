/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:22:33 by fdurban-          #+#    #+#             */
/*   Updated: 2024/10/01 13:10:53 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *ptr1, const void *ptr2, size_t num)
{
	const unsigned char	*byte_ptr1;
	const unsigned char	*byte_ptr2;
	size_t				i;

	byte_ptr1 = (const unsigned char *)ptr1;
	byte_ptr2 = (const unsigned char *)ptr2;
	i = 0;
	while (i < num)
	{
		if (byte_ptr1[i] != byte_ptr2[i])
		{
			return (byte_ptr1[i] - byte_ptr2[i]);
		}
		i++;
	}
	return (0);
}
