/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise_redir_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 06:42:13 by mateo             #+#    #+#             */
/*   Updated: 2024/05/25 18:17:55 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	redir_code returns the redirection token code 
	when given the redirection string
	otherwise, it returns TOKEN_TEMP */
int	redir_code(char *str)
{
	if (ft_strcmp(str, ">") == 0)
		return (TOKEN_OUTPUT);
	else if (ft_strcmp(str, ">") == 0)
		return (TOKEN_INPUT);
	else if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_APPEND);
	else if (ft_strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC);
	return (TOKEN_TEMP);
}

/*	redir_str processes redirection operator at the start of token->str
	- allocates and returns the string containing just the redirection operator */
char	*redir_str(t_token *token)
{
	char	*redir;
	
	if (token->str[0] == '>' && token->str[1] == '>')
		redir = ft_strdup(">>");
	else if (token->str[0] =='>' && token->str[1] != '>')
		redir = ft_strdup(">");
	else if (token->str[0] == '<' && token->str[1] == '<')
		redir = ft_strdup("<<");
	else if (token->str[0] == '<' && token->str[1] != '<')
		redir = ft_strdup("<");
	token->code = redir_code(redir);
	return (redir);
}

/*	str_is_num returns 1 if str is composed of only digits
	0 otherwise */
int str_is_num(char *str)
{
	while (*str)
	{
		if (ft_isdigit(*str) == 0)
			return (0);
		str++;
	}
	return (1);
}
