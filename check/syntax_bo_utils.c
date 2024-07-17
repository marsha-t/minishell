/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_bo_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 21:55:21 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/16 17:15:27 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	pipe_mid returns 0 if line[i] (a pipe) is in the middle of a line
	- if pipe is at end of line, it prompts for more input and returns 1 */
// work in progress: incomplete line
int	pipe_mid(int i, char *line)
{
	i++;
	while (line[i] == 32 || line[i] == 9)
		i++;
	// if (line[i] == '\0')
	// 	return (write(1, ">\n", 2), 1); // work in progress: how to read another line
	return (0);
}

/*	and_mid returns 0 if only whitespace follow line[i]
	i.e., && ends the line (with or without whitespace)
	- before returning 1, it prompts for more input*/
// work in progress: incomplete line
int	and_mid(int i, char *line)
{
	i = i + 2;
	while (line[i] == 32 || line[i] == 9)
		i++;
	// if (line[i] == '\0')
	// 	return (write(1, ">\n", 2), 1);
	return (0);
}

/*	or_mid returns 0 if only whitespace follow line[i]
	i.e., || ends the line (with or without whitespace)
	- before returning 1, it prompts for more input*/
// work in progress: incomplete line
int	or_mid(int i, char *line)
{
	i = i + 2;
	while (line[i] == 32 || line[i] == 9)
		i++;
	// if (line[i] == '\0')
	// 	return (write(1, ">\n", 2), 1);
	return (0);
}
