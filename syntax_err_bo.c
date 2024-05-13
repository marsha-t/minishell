/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_err_bo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 14:25:58 by ryagoub           #+#    #+#             */
/*   Updated: 2024/05/13 11:08:59 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int and_mid(int i, char *line)
{
	i = i + 2;
	while (line [i] == 32 || line[i] == 9)
		i++;
	if (line[i] == '\0')
		return (write(1, ">\n", 2), 0);
	return (1);

}
int check_and(char *line)
{
	int i;
	int	j;

	i = 0;
	j= 0;
	while (line[i] != '\0')
	{
		if(line[0] == '&' && line[1] == '&')
			return(write(2, "minishell: syntax error near unexpected token `&&'\n",46 ),0);
		j = i;
		while (line [i] == 32 || line[i] == 9 )
		{
			i++;
			if (line[i] == '&' && line[i + 1]== '&' && j == 0)
				return(write(2, "minishell: syntax error near unexpected token `&&'\n",46 ),0);
		}
		if(line[i] == '&' && line[i + 1] == '&')
		{
			if(and_mid(i, line) == 0)
				return(0);
		}
		i++;
	}
	return(1);
}

int or_mid(int i, char *line)
{
	i = i + 2;
	while (line [i] == 32 || line[i] == 9)
		i++;
	if (line[i] == '\0')
		return (write(1, ">\n", 2), 0);
	return (1);

}
int check_or(char *line)
{
	int i;
	int	j;

	i = 0;
	j= 0;
	while (line[i] != '\0')
	{
		if(line[0] == '|' && line[1] == '|')
			return(write(2, "minishell: syntax error near unexpected token `||'\n",46 ),0);
		j = i;
		while (line [i] == 32 || line[i] == 9 )
		{
			i++;
			if (line[i] == '|' && line[i + 1]== '|' && j == 0)
				return(write(2, "minishell: syntax error near unexpected token `||'\n",46 ),0);
		}
		if(line[i] == '|' && line[i + 1] == '|')
		{
			if(or_mid(i, line) == 0)
				return(0);
		}
		i++;
	}
	return(1);
}

int check_op_para(char *line)
{
	int i;
	char open_p;
	char close_p;

	open_p = '(';
	close_p = ')';
	i= 0;
	while(line[i] != '\0')
	{
		if(line[i] == open_p)
		{
			i++;
			while(line[i] != '\0' && line[i] != close_p)
				i++;
			if(line[i] == '\0')
				return(write(1, ">\n", 2), 0);
			else
				i++;
		}
		else
			i++;
	}
	return(1);
}
int check_close_para(char *line)
{
	int i;
	int open_p;
	char close_p;

	close_p = ')';
	open_p = 0;
	i= 0;
	while(line[i] != '\0')
	{
		if (line[i] == '(')
			open_p++;
		if(line[i] == close_p && open_p == 0)
		{
			write(1, "minishell: syntax error near unexpected token `)'\n", 51);
			return(0);
		}
		else
			i++;
	}
	return(1);
}
