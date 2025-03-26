/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 15:17:07 by fdurban-          #+#    #+#             */
/*   Updated: 2024/10/15 16:55:31 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putnbr(long n)
{
	int		len;
	char	c;

	len = 0;
	if (n < 0)
	{
		len += write(1, "-", 1);
		n = -n;
	}
	if (n >= 10)
	{
		len += ft_putnbr(n / 10);
	}
	c = (n % 10) + '0';
	len += write(1, &c, 1);
	return (len);
}

int	ft_putnbr_hex(unsigned long num, int uppercase)
{
	int		len;
	char	hex_digits;

	len = 0;
	if (uppercase)
		hex_digits = "0123456789ABCDEF"[num % 16];
	else
		hex_digits = "0123456789abcdef"[num % 16];
	if (num >= 16)
		len += ft_putnbr_hex(num / 16, uppercase);
	len += ft_putchar(hex_digits);
	return (len);
}

int	ft_putstr(char *s)
{
	int	i;

	i = 0;
	if (!s)
	{
		i += write(1, "(null)", 6);
		return (i);
	}
	while (s[i] != '\0')
	{
		write(1, &s[i], 1);
		i++;
	}
	return (i);
}

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_print_memory(void *mem)
{
	int					len;
	unsigned long		adress;

	len = 0;
	adress = (unsigned long)mem;
	if (!mem)
	{
		len += write(1, "(nil)", 5);
		return (len);
	}
	len += write(1, "0x", 2);
	len += ft_putnbr_hex(adress, 0);
	return (len);
}
