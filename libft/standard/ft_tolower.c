/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:39:33 by fdurban-          #+#    #+#             */
/*   Updated: 2024/09/27 18:24:25 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_tolower(int c)
{
	if ((unsigned char)c >= 65 && (unsigned char)c <= 90)
	{
		c = c + 32;
	}
	return (c);
}
