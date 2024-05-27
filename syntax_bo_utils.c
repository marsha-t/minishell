/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_bo_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 21:55:21 by ryagoub           #+#    #+#             */
/*   Updated: 2024/05/13 22:03:11 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int para_found(int i, char *line, int op_count)
{
	i++;
	while(line[i] == '(')
	{
		i++;
		op_count++;
	}
	while (line[i] != '\0' && op_count != 0)
	{
		i++;
		if(line[i] == ')')
			op_count--;
	}
	if (line[i] == '\0')
		return (write(1, ">\n", 2), 0);
	else
		i++;
	return(1);
}
