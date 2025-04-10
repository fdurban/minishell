/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   automats.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernando <fernando@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:59:56 by fdurban-          #+#    #+#             */
/*   Updated: 2025/04/10 17:52:33 by fernando         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void    parse(char **readline)
{
    char    c;
    int     i;
    t_state state;

    i = 0;
    state = STATE_START;
    while (readline[i])
    {
        c = readline[i];
        i++;
    }
    
}