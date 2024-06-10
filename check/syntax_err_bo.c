/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_err_bo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 14:25:58 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/02 17:28:11 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	check_and returns 1 if
	- line starts with && (with or without whitespace)
	- prompts for more input if || ends line (with or without whitespace)*/
int	check_and(char *line)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (line[i] != '\0')
	{
		if (line[0] == '&' && line[1] == '&')
			return (write(2, "minishell: syntax error near unexpected token `&&'\n", 46), 1);
		j = i;
		while (line[i] == 32 || line[i] == 9)
		{
			i++;
			if (line[i] == '&' && line[i + 1] == '&' && j == 0)
				return (write(2, "minishell: syntax error near unexpected token `&&'\n", 46), 1);
		}
		if (line[i] == '&' && line[i + 1] == '&')
		{
			if (and_mid(i, line) == 1)
				return (1);
		}
		i++;
	}
	return (0);
}

/*	check_or returns 1 if
	- || starts line (with or without whitespace)
	- prompts for more input if || ends line (with or without whitespace)*/
int	check_or(char *line)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (line[i] != '\0')
	{
		if (line[0] == '|' && line[1] == '|')
			return (write(2, "minishell: syntax error near unexpected token `||'\n", 46), 1);
		j = i;
		while (line[i] == 32 || line[i] == 9)
		{
			i++;
			if (line[i] == '|' && line[i + 1] == '|' && j == 0)
				return (write(2, "minishell: syntax error near unexpected token `||'\n", 46), 1);
		}
		if (line[i] == '|' && line[i + 1] == '|')
		{
			if (or_mid(i, line) == 1)
				return (1);
		}
		i++;
	}
	return (0);
}

/*	check_op_para returns 1 if open parenthesis is not closed
	in this case, there is a prompt for more input */
int	check_op_para(char *line)
{
	int		i;
	int		op_count;

	i = 0;
	op_count = 1;
	while (line[i] != '\0')
	{
		if (line[i] == '(')
		{
			i++;
			while (line[i++] == '(')
				op_count++;
			while (line[i] != '\0' && op_count != 0)
			{
				i++;
				if (line[i] == ')')
					op_count--;
			}
			if (line[i] == '\0')
				return (write(1, ">\n", 2), 1);
		}
		else
			i++;
	}
	return (0);
}

/*	check_close_para returns 1 if a closing para exists without an opening one*/
int	check_close_para(char *line)
{
	int		i;
	int		open_p;
	int		close_p;

	open_p = 0;
	close_p = 0;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '(')
			open_p++;
		if (line[i] == ')')
			close_p++;
		i++;
	}
	if (close_p > open_p)
	{
		write(2, "minishell: syntax error near unexpected token `)'\n", 51);
		return (1);
	}
	return (0);
}
