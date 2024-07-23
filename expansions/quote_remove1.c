/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_remove1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 05:54:12 by mateo             #+#    #+#             */
/*   Updated: 2024/07/23 14:52:30 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	remove_quote_str removes quotes from a given string
	- retains quotes that are quoted
	 e.g., single quote inside double or double inside single
	- frees original str
	- malloc error from strdup or strjoin will 
	result in unquoted_str being NULL */
char	*remove_quote_str(char *str)
{
	char	quote;
	int		i;
	int		start;

	if (remove_quote_str_init(str, &quote, &i, &start) == 1)
		return (NULL);
	if (remove_quote_str_while(&str, i, quote, &start) == 1)
		return (0);
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
	t_list	*curr_arg;

	node->cmd = remove_quote_str(node->cmd);
	if (!node->cmd)
		return (1);
	if (node->n_args > 0)
	{
		curr_arg = node->args;
		while (curr_arg)
		{
			curr_arg->content = remove_quote_str(curr_arg->content);
			if (!curr_arg->content)
				return (1);
			curr_arg = curr_arg->next;
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
