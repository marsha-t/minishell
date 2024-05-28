/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise_redir_tokens.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 06:43:57 by mateo             #+#    #+#             */
/*   Updated: 2024/06/03 17:40:57 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	check_redir_token returns -1 if there isn't an unquoted redirection operator
	otherwise, it returns i (index of first unquoted redirection operator) */
int	check_redir_token(char *str)
{
	int	i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
		{
			if (quote == 0)
				quote = str[i];
			else if (quote != 0 && quote == str[i])
				quote = 0;
		}
		else if ((str[i] == '>' || str[i] == '<') && quote == 0)
			return (i);
		i++;
	}
	return (-1);
}

/*	pre_redir_token generates a new token for the part before the redirection operator
	- categorises this token as IO_NUM or ARG */
int	pre_redir_token(t_token **token, int redir_start)
{
	char	*pre;
	char	*remainder;
	
	pre = strdup_range(&(*token)->str[0], &(*token)->str[redir_start - 1]); // -1 because redir_start points to redir
	if (!pre)
		return (1);
	remainder = strdup_range(&(*token)->str[redir_start], &(*token)->str[ft_strlen((*token)->str) - 1]);
	if (!remainder)
		return (1);
	free((*token)->str);
	(*token)->str = pre;
	if (str_is_num(pre) == 1)
		(*token)->code = TOKEN_IONUM;
	else
		(*token)->code = TOKEN_ARG;
	if (insert_token(token, remainder, TOKEN_TEMP) == 1)
		return (1);
	return (0);
}

/*	redir_file_token
	- splits str into redirection (using redir_str) and remainder 
		- remainder contains file and may contain other redirections
	- change current token into redirection token
	- if there are no other redirection operations, insert file token
	- else, insert file token and TOKEN_TEMP token containing remainder 
	NOTE: for now, assumes what follows heredoc should be a file node*/
int	redir_file_token(t_token **token)
{
	int		start;
	char	*temp;
	char	*remainder;
	
	temp = redir_str(*token);
	if (!temp)
		return (1);
	start = ft_strlen(temp);
	if (ft_strlen((*token)->str) == (size_t)start)
	{
		free((*token)->str);
		(*token)->str = temp;
		return (0);
	}
	remainder = strdup_range(&(*token)->str[start], &(*token)->str[ft_strlen((*token)->str) - 1]);
	if (!remainder)
	{
		free(temp);
		return (1);
	}
	free((*token)->str);
	(*token)->str = temp;
	start = check_redir_token(remainder);
	if (start == -1)
	{
		if (insert_token(token, remainder, TOKEN_FILE) == 1)
			return (1);
		*token = (*token)->next;
	}
	else
	{
		if (insert_token(token, strdup_range(remainder, &remainder[start - 1]), TOKEN_FILE) == 1)
		{
			free(remainder);
			return (1);
		}
		if (insert_token(token, strdup_range(&remainder[start], &remainder[ft_strlen(remainder) - 1]), TOKEN_TEMP) == 1)
		{
			free(remainder);
			return (1);
		}
		free(remainder);
	}
	return (0);
}

/*	split_redir_token
	- first parses what comes before the redirection operator (if any);
	- if only redirection operator left in token, updates code of token
	- else use redir_file_token to generate (first set of) redirection and file tokens */
int	split_redir_token(t_token **token, int redir_start)
{
	int	len;
	
	if (redir_start != 0)
	{
		if (pre_redir_token(token, redir_start) == 1)
			return (1);
	}
	len = ft_strlen((*token)->str);
	if (len == 1 || (len == 2 && (*token)->str[0] == (*token)->str[1]))
		(*token)->code = redir_code((*token)->str);
	else if (redir_file_token(token) == 1)
		return (1);
	return (0);
}

/*	insert_default_io_token inserts IO_NUM token ahead of redirection tokens that omitted it
	- default IO number for input is 0
	- default IO number for output and append is 1 */
int	insert_default_io_token(t_token **current, int redir_code)
{
	if (redir_code == TOKEN_INPUT)
	{
		if (insert_token(current, "0", TOKEN_IONUM) == 1)
			return (1);
	}
	else if (redir_code == TOKEN_OUTPUT)
	{
		if (insert_token(current, "1", TOKEN_IONUM) == 1)
			return (1);
	}
	else if (redir_code == TOKEN_APPEND)
	{
		if (insert_token(current, "1", TOKEN_IONUM) == 1)
			return (1);
	}
	*current = (*current)->next; // skip past redirection and to file node // assume syntax checks are done
	return (0);
}