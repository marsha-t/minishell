/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_wc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 05:36:10 by mateo             #+#    #+#             */
/*   Updated: 2024/07/11 05:51:44 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	contain_wc checks whether str contains a wildcard expansion 
	i.e., unquoted asterisk (*) */
int contain_wc(char *str)
{
	int		i;
	char	quote;
	
	i = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == 39 || str[i] == '"')
		{
			quote = str[i];
			i++;
			while (str[i] != '\0' && str[i] != quote)
				i++;
		}
		else if (str[i] == 42)
			return (0);
		i++;
	}
	return (1);
}

/*	file_list_check_wc calls contain_wc and expand_wildcard
	for each file provided in file linked list
	*/
// work in progress: can wildcards work in heredocs?
int	file_list_check_wc(t_ast *node, t_shell *shell, int code)
{
	t_file	*curr_file;

	if (code == TOKEN_INPUT)
		curr_file = node->input_list;
	else if (code == TOKEN_HEREDOC)
		curr_file = node->heredoc_list;
	else if (code == TOKEN_OUTPUT)
		curr_file = node->output_list;
	while (curr_file)
	{
		if (contain_wc(curr_file->file_name) == 0)
		{
			if (expand_wildcard_file(shell->directory_contents, node, curr_file->file_name, code))
				return (1);
		}
		curr_file = curr_file->next;
	}
	return (0);
}


/*	check_wc_expansion checks whether wildcard expansions are needed
	- loads and eventually frees shell->directory_contents
	- checks strings in cmd, args and files
	- if needed, expands them
	- returns 1 if errors with expansion
	*/
// work in progress: need to cater for ./t*c i.e., to cater for ./
int	check_wc_expansion(t_ast *node, t_shell *shell)
{
	t_list	*curr_arg;

	shell->directory_contents= create_conts_list();
	if (contain_wc(node->cmd) == 0)
	{
		if (expand_wildcard_cmd(shell->directory_contents, node) == 1)
			return (free_conts_list(shell->directory_contents), 1);
	}
	if (node->n_args > 0)
	{
		curr_arg = node->args;
		while (curr_arg)
		{
			if (contain_wc(curr_arg->content) == 0)
			{
				if (expand_wildcard_arg(shell->directory_contents, node, curr_arg->content) == 1)
					return (free_conts_list(shell->directory_contents), 1);
			}
			curr_arg = curr_arg->next;
		}
	}
	if (node->input_list)
	{
		if (file_list_check_wc(node, shell, TOKEN_INPUT) == 1)
			return (free_conts_list(shell->directory_contents), 1);
	}
	if (node->heredoc_list)
	{
		if (file_list_check_wc(node, shell, TOKEN_HEREDOC) == 1)
			return (free_conts_list(shell->directory_contents), 1);
	}
	if (node->output_list)
	{
		if (file_list_check_wc(node, shell, TOKEN_OUTPUT) == 1)
			return (free_conts_list(shell->directory_contents), 1);
	}
	free_conts_list(shell->directory_contents);
	return (0);
}
