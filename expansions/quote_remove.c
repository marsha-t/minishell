/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_remove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 05:54:12 by mateo             #+#    #+#             */
/*   Updated: 2024/06/13 05:35:28 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	ft_strjoin_free joins str as ft_strjoin would except
	- returns s2 if s1 is null
	- frees s1 and s2*/
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*ptr;
	size_t	tot_len;
	int		i;
	int		j;
	
	if (!s1 || !s2)
		return (free_num(2, s1, s2), 0);
	// if (!s1) 
	// 	return (s2);
	// if (!s2)
	// 	return (0);
	tot_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	ptr = (char *)malloc(sizeof(char) * tot_len);
	if (!ptr)
		return (0);
	i = 0;
	j = 0;
	while (s1[j])
		ptr[i++] = s1[j++];
	j = 0;
	while (s2[j])
		ptr[i++] = s2[j++];
	ptr[i] = '\0';
	free_num(2, s1, s2);
	// free(s1);
	// free(s2);
	return (ptr);
}

/*	remove_quote removes quotes from a given string
	- retains quotes that are quoted e.g., single quote inside double or double inside single
	- frees original str
	- malloc error from strdup or strjoin will result in unquoted_str being NULL */
// work in progress: check whether updated strjoin_free still works for this function
char	*remove_quote_str(char *str)
{
	int	i;
	int	start;
	char	quote;
	char	*unquoted_str;

	i = 0;
	start = 0;
	quote = 0;
	unquoted_str = 0; 
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
		{
			if (quote == 0)
			{
				quote = str[i];
				if (i != 0)
					unquoted_str = strdup_range(&str[0], &str[i-1]);	
				start = i + 1;
			}
			else if (quote != 0 && quote == str[i])
			{
				quote = 0;
				unquoted_str = ft_strjoin_free(unquoted_str, strdup_range(&str[start], &str[i - 1]));
				start = i + 1;
			}
		}
		i++;
	}
	unquoted_str = ft_strjoin_free(unquoted_str, strdup_range(&str[start], &str[i - 1]));
	free(str);
	return (unquoted_str);
}

/*	remove_quote_node removes quotes from command node:
	- removed from cmd, arg, file*/
	// work in progress depending on structure of ast node for args, input and output_append
int	remove_quote_node(t_ast *node)
{
	int i;
	
	node->cmd = remove_quote_str(node->cmd);
	if (!node->cmd)
	{
		// error msg: malloc error
		return (1);
	}
	if (n_args > 0)
	{
		i = 0;
		while (i < n_args)
		{
			node->args[i] = remove_quote_str(node->cmd);
			if (!node->args[i])
			{
				// error msg: malloc error
				return (1);
			}
		}
	}
	if (n_input > 0)
	{
		
	}
	if (n_output_append > 0)
	{
		
	}
}
