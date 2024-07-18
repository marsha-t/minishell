/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_bo_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 21:55:21 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/18 16:41:15 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	is_direct_double returns 0 if >> or << */
int	is_direct_double(char a, char b)
{
	if ((a == '>' && b == '>') || (a == '<' && b == '<'))
		return (0);
	return (1);
}

/*	only_whitespace_left checks whether 
	there is only whitespace left in line (returns 0 if so) */
int	only_whitespace_left(char *line, int i)
{
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i] == '\0')
		return (0);
	return (1);
}

/*	pipe_mid returns 0 if line[i] (a pipe) is in the middle of a line
	- if pipe is at end of line, returns 1 */
int	pipe_mid(int i, char *line)
{
	i++;
	while (line[i] == 32 || line[i] == 9)
		i++;
	if (line[i] == '\0')
		return (1);
	return (0);
}

/*	and_mid returns 1 if only whitespace follow line[i]
	i.e., && ends the line (with or without whitespace) */
int	and_mid(int i, char *line)
{
	i = i + 2;
	while (line[i] == 32 || line[i] == 9)
		i++;
	if (line[i] == '\0')
		return (1);
	return (0);
}

/*	or_mid returns 1 if only whitespace follow line[i]
	i.e., || ends the line (with or without whitespace)
	*/
int	or_mid(int i, char *line)
{
	i = i + 2;
	while (line[i] == 32 || line[i] == 9)
		i++;
	if (line[i] == '\0')
		return (1);
	return (0);
}
