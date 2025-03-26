/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdurban- <fdurban-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:14:41 by fdurban-          #+#    #+#             */
/*   Updated: 2024/10/01 13:14:18 by fdurban-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *string)
{
	char		*cpy;
	int			len;
	int			i;

	len = ft_strlen(string);
	cpy = malloc(len + 1);
	if (cpy == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		cpy[i] = string[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}
