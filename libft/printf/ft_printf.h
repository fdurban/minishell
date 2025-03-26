/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:06:40 by fdurban-          #+#    #+#             */
/*   Updated: 2024/10/28 11:21:25 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int	ft_putchar(char c);
int	ft_putnbr(long n);
int	ft_putstr(char *s);
int	ft_putnbr_hex(unsigned long num, int uppercase);
int	ft_print_memory(void *mem);
int	ft_printf(const char *str, ...);
int	check_method(const char *str, va_list args);
int	process_format(const char specifier, va_list args);

#endif