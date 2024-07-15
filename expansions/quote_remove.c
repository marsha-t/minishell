/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_remove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 05:54:12 by mateo             #+#    #+#             */
/*   Updated: 2024/07/15 07:48:59 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	remove_quote_join splits str into what is inside the quote and after it
	then combines it
	- updates start and i as well */
char	*remove_quote_join(char *str, char *before_quote, int *start, int *i)
{
	char	*in_quote;
	char	*after_quote;
	char	*combined;

	if (*start == *i)
		in_quote = ft_strdup("");
	else
		in_quote = strdup_range(&str[*start], &str[*i - 1]);
	if (!in_quote)
		return (err_printf("minishell: malloc error: in_quote\n"), NULL);
	if (*i == (int)ft_strlen(str) - 1)
		after_quote = ft_strdup("");
	else
		after_quote = strdup_range(&str[*i + 1], &str[ft_strlen(str) - 1]);
	if (!after_quote)
	{
		free(in_quote);
		return (err_printf("minishell: malloc error: after_quote\n"), NULL);
	}
	free(str);
	*i = ft_strlen(before_quote) + ft_strlen(in_quote) - 1;
	combined = strjoin_num_free(3, before_quote, in_quote, after_quote);
	if (!combined)
		return (free_num(3, before_quote, in_quote, after_quote), NULL);
	*start = *i;
	return (combined);
}

/*	remove_quote removes quotes from a given string
	- retains quotes that are quoted e.g., single quote inside double or double inside single
	- frees original str
	- malloc error from strdup or strjoin will result in unquoted_str being NULL */
char	*remove_quote_str(char *str)
{
	char	quote;
	int		i;
	int		start;
	char	*before_quote;

	if (!str)
		return (err_printf("minishell: malloc error: ft_strdup\n"), NULL);
	quote = 0;
	i = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
		{
			if (quote == 0)
			{
				quote = str[i];
				if (i == 0)
					before_quote = ft_strdup("");
				else
					before_quote = strdup_range(&str[0], &str[i - 1]);
				if (!before_quote)
					return (err_printf("minishell: malloc error: before_quote\n"), NULL);
				start = i + 1;
			}
			else if (quote == str[i])
			{
				str = remove_quote_join(str, before_quote, &start, &i);
				if (!str)
					return (NULL);
				quote = 0;
			}
		}
		i++;
	}
	return (str);
}

/*	remove_quote_file removes quotes from file names
	- works with input_list, output_list and heredoc_list */
int	remove_quote_file(t_file *file)
{
	t_file	*current;

	current = file;
	while (current)
	{
		current->file_name = remove_quote_str(current->file_name);
		if (!current->file_name)
			return (1);
		current = current->next;
	}
	return (0);
}

/*	remove_quote_node removes quotes from command node:
	- removed from cmd, arg, input_file, output_file and heredoc_file*/
int	remove_quote_node(t_ast *node)
{
	int	i;

	node->cmd = remove_quote_str(node->cmd);
	if (!node->cmd)
		return (1);
	if (node->n_args > 0)
	{
		i = 0;
		while (i < node->n_args)
		{
			node->args->content = remove_quote_str(node->args->content);
			if (!node->args->content)
				return (1);
			i++;
		}
	}
	if (node->input_list && remove_quote_file(node->input_list) == 1)
		return (1);
	if (node->output_list && remove_quote_file(node->output_list) == 1)
		return (1);
	if (node->heredoc_list && remove_quote_file(node->heredoc_list) == 1)
		return (1);
	return (0);
}
