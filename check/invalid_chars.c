/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invalid_chars.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 08:41:55 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/02 12:46:19 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
