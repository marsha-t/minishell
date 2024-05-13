/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_err.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:28:19 by ryagoub           #+#    #+#             */
/*   Updated: 2024/05/13 11:09:34 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int check_direct(char *line)
{
	int i;
	i = 0;
	while (line[i] != '\0')
	{
		if((line[i] == '>' || line[i] == '<')&&(line[i+1] == '\0' ))
			return(0);
		else if(((line[i] == '>' && line[i+1] == '>') || (line[i] == '<' && line[i+1] == '<')) && line[i+ 2] == '\0')
			return(0);
		i++;
	}
	return(1);

}
int check_quotes(char *line)
{
	int i;
	char qoute_t;
	i= 0;
	while(line[i] != '\0')
	{
		if(line[i] == '"' || line[i] == 39)
		{
			qoute_t = line[i];
			i++;
			while(line[i] != '\0' && line[i] != qoute_t)
				i++;
			if(line[i] == '\0')
				return(0);
			else
				i++;
		}
		else
			i++;
	}
	return(1);
}
int pipe_mid(int i, char *line)
{
	i++;
	while (line [i] == 32 || line[i] == 9)
		i++;
	if (line[i] == '\0')
		return (write(1, ">\n", 2), 0);
	return (1);

}
int check_pipes(char *line)
{
	int i;
	int	j;

	i = 0;
	j= 0;
	while (line[i] != '\0')
	{
		if(line[0] == '|' && line [1] != '|')
			return(write(2, "minishell: syntax error near unexpected token `|'\n",46 ),0);
		j = i;
		while ((line [i] == 32 || line[i] == 9 ))
		{
			i++;
			if (line[i] == '|' && line [i + 1] != '|' && j == 0)
				return(write(2, "minishell: syntax error near unexpected token `|'\n",46 ),0);
		}
		if(line[i] == '|' && line [i + 1] != '|' && line[i - 1] != '|')
		{
			if(pipe_mid(i, line) == 0)
				return(0);
		}
		i++;
	}
	return(1);
}

int check_all(char *line)
{
	if(check_quotes(line) == 1 && check_pipes(line) == 1 && check_direct(line) == 1
		&& check_and(line) == 1 && check_or(line) == 1 && check_op_para(line) == 1
		&&check_close_para(line) == 1)
		return(1);
	else
		return(0);
}
