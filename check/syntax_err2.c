/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_err2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 14:25:58 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/18 17:03:51 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
			return (err_syntax("||", 1));
		j = i;
		while (line[i] == 32 || line[i] == 9)
		{
			i++;
			if (line[i] == '|' && line[i + 1] == '|' && j == 0)
				return (err_syntax("||", 1));
		}
		if (line[i] == '|' && line[i + 1] == '|')
		{
			if (or_mid(i, line) == 1)
				return (err_syntax("||", 1));
		}
		i++;
	}
	return (0);
}

/*	check_op_para returns 1 if open parenthesis is not closed
	and returns error 
	- ignores if parentheses are in quotes */
int	check_op_para(char *line)
{
	int		i;
	int		op_count;

	i = 0;
	op_count = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\'' || line[i] == '\"')
			skip_quoted(line, &i);
		else if (line[i] == '(')
		{
			i++;
			op_count++;
			while (op_count != 0)
			{
				if (check_op_para_count(line, &i, &op_count) == 1)
					return (err_syntax("(", 1));
			}
		}
		else
			i++;
	}
	return (0);
}

/*	check_close_para returns 1 
	if a closing para exists without an opening one
	- ignores if parentheses are in quotes */
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
		if (line[i] == '\'' || line[i] == '\"')
			skip_quoted(line, &i);
		else
		{
			if (line[i] == '(')
				open_p++;
			else if (line[i] == ')')
				close_p++;
			i++;
		}
	}
	if (close_p > open_p)
		return (err_syntax(")", 1));
	return (0);
}

/*	check_empty_para returns 1 if there is () that is unquoted */
int	check_empty_para(char *line)
{
	int		i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\'' || line[i] == '\"')
			skip_quoted(line, &i);
		else if (line[i] == '(' && line[i + 1] == ')')
			return (err_syntax(")", 1));
		else
			i++;
	}
	return (0);
}
