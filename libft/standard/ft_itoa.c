/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:27:51 by fdurban-          #+#    #+#             */
/*   Updated: 2024/10/01 13:41:07 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	countdigits(int n)
{
	int	count;

	count = 0;
	if (n <= 0)
	{
		count = 1;
	}
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int		counter;
	char	*result;
	long	nb;

	nb = n;
	counter = countdigits(nb);
	result = malloc(counter + 1);
	if (!result)
		return (NULL);
	result[counter] = '\0';
	if (nb == 0)
		result[0] = '0';
	if (nb < 0)
	{
		result[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		result[--counter] = (nb % 10) + '0';
		nb /= 10;
	}
	return (result);
}
