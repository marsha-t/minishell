/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invalid_chars.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 08:41:55 by ryagoub           #+#    #+#             */
/*   Updated: 2024/05/15 08:42:57 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_single_and(char *line)
{
    int i;

    i = 0;
    while (line[i] != '\0')
    {
        if(line[i] == '&' && line [i+1] != '&')
        {
            write(2, "Error\n", 6);
            return(0);
        }
        i++;
    }
    return(1);
}
