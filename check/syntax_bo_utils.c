/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_bo_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 21:55:21 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/02 17:28:41 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	pipe_mid returns 1 if line[i] (a pipe) is in the middle of a line
	- if pipe is at end of line, it prompts for more input and returns 0 */
int	pipe_mid(int i, char *line)
{
	i++;
	while (line[i] == 32 || line[i] == 9)
		i++;
	if (line[i] == '\0')
		return (write(1, ">\n", 2), 0); // work in progress: how to read another line
	return (1);
}

/*	and_mid returns 0 if only whitespace follow line[i]
	i.e., && ends the line (with or without whitespace)
	- before returning 0, it prompts for more input*/
int	and_mid(int i, char *line)
{
	i = i + 2;
	while (line[i] == 32 || line[i] == 9)
		i++;
	if (line[i] == '\0')
		return (write(1, ">\n", 2), 0);
	return (1);
}

/*	or_mid returns 0 if only whitespace follow line[i]
	i.e., || ends the line (with or without whitespace)
	- before returning 0, it prompts for more input*/
int	or_mid(int i, char *line)
{
	i = i + 2;
	while (line[i] == 32 || line[i] == 9)
		i++;
	if (line[i] == '\0')
		return (write(1, ">\n", 2), 0);
	return (1);
}
