/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_err.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:28:19 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/07 15:43:26 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	check_direct returns 1 if
	-> followed by < and vice versa
	- line ends with >, >>, < or << */
int	check_direct(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '>' && line[i + 1] == '<') 
			return (err_printf("minishell: syntax error near unexpected token `<'\n"), 1);
		else if  (line[i] == '<' && line[i + 1] == '>')
			return (err_printf("minishell: syntax error near unexpected token `newline'\n"), 1);
		else if (((line[i] == '>' && line[i + 1] == '>') || (line[i] == '<' && line[i + 1] == '<')))
		{
			i = i + 2;
			while (line [i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '\0')
				return (err_printf("minishell: syntax error near unexpected token `newline'\n"), 1);
		}
		else if ((line[i] == '>' || line[i] == '<' ))
		{
			i++;
			while (line [i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '\0')
				return (err_printf("minishell: syntax error near unexpected token `newline'\n"), 1);
		}
		i++;
	}
	return (0);
}

/*	check_quotes returns 1 if mismatched quotes */
int	check_quotes(char *line)
{
	int		i;
	char	quote_t;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '"' || line[i] == 39)
		{
			quote_t = line[i];
			i++;
			while (line[i] != '\0' && line[i] != quote_t)
				i++;
			if (line[i] == '\0')
				return (err_printf("minishell: syntax error near mismatched quotes\n"), 1);
			else
				i++;
		}
		else
			i++;
	}
	return (0);
}

/*	check_pipes returns 0 if
	- | starts line (with or without whitespace before |)
	- prompts for more input if | ends line (with or without whitespace) */
int	check_pipes(char *line)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (line[i] != '\0')
	{
		if (line[0] == '|' && line [1] != '|')
			return (err_printf("minishell: syntax error near unexpected token `|'\n"), 1);
		j = i;
		while ((line[i] == 32 || line[i] == 9))
		{
			i++;
			if (line[i] == '|' && line [i + 1] != '|' && j == 0)
				return (err_printf("minishell: syntax error near unexpected token `|'\n"), 1);
		}
		if (line[i] == '|' && line [i + 1] != '|' && line[i - 1] != '|')
		{
			if (pipe_mid(i, line) == 1)
				return (1);
		}
		i++;
	}
	return (0);
}

/*	check_all returns 0 if all checks are passed */
int	check_all(char *line)
{
	if (check_quotes(line) == 0 && check_pipes(line) == 0
		&& check_direct(line) == 0 && check_and(line) == 0
		&& check_or(line) == 0 && check_op_para(line) == 0
		&& check_close_para(line) == 0)
		return (0);
	else
		return (1);
}
