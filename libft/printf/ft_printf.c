/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:41:08 by fdurban-          #+#    #+#             */
/*   Updated: 2024/10/15 16:04:38 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	process_format(const char specifier, va_list args)
{
	int	len;

	len = 0;
	if (specifier == 'c')
		len += ft_putchar((char)va_arg(args, int));
	else if (specifier == 's')
		len += ft_putstr(va_arg(args, char *));
	else if (specifier == 'd' || specifier == 'i')
		len += ft_putnbr(va_arg(args, int));
	else if (specifier == 'u')
		len += ft_putnbr(va_arg(args, unsigned int));
	else if (specifier == '%')
		len += ft_putchar('%');
	else if (specifier == 'x')
		len += ft_putnbr_hex(va_arg(args, unsigned int), 0);
	else if (specifier == 'X')
		len += ft_putnbr_hex(va_arg(args, unsigned int), 1);
	else if (specifier == 'p')
		len += ft_print_memory(va_arg(args, void *));
	return (len);
}

int	check_method(const char *str, va_list args)
{
	int				len;

	len = 0;
	while (*str != '\0')
	{
		if (*str == '%')
		{
			if (*(str + 1) == '%')
			{
				len += write(1, "%", 1);
				str++;
			}
			else if (*(str + 1) == '\0')
				return (-1);
			else
			{
				len += process_format(*(str + 1), args);
				str++;
			}
		}
		else
			len += write(1, str, 1);
		str++;
	}
	return (len);
}

int	ft_printf(const char *str, ...)
{
	int				len;
	va_list			args;

	len = 0;
	va_start(args, str);
	len += check_method(str, args);
	va_end(args);
	return (len);
}
