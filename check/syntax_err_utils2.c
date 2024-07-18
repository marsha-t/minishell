/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_err_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 16:58:55 by mateo             #+#    #+#             */
/*   Updated: 2024/07/18 16:59:02 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	skip_quoted is called when a quote has been reached
	- skips everything until the ending quote
	- updates index to after the ending quote */
void	skip_quoted(char *line, int *i)
{
	char	quote;

	quote = line[*i];
	(*i)++;
	while (line[*i] != quote && line[*i] != '\0')
		(*i)++;
	(*i)++;
}

/*	check_op_para_count updates op_count and index
	once an open bracket has been found 
	- returns 1 if error */
int	check_op_para_count(char *line, int *i, int *op_count)
{
	if (line[*i] == '(')
		(*op_count)++;
	else if (line[*i] == ')')
		(*op_count)--;
	else if (line[*i] == '\0')
		return (1);
	(*i)++;
	return (0);
}
