/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_wc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 05:36:10 by mateo             #+#    #+#             */
/*   Updated: 2024/07/15 07:38:34 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	contain_wc checks whether str contains a wildcard expansion 
	i.e., unquoted asterisk (*) */
int	contain_wc(char *str)
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

/*	file_list_check_wc calls contain_wc and expand_wc_file
	for input/output lists
	- return 1 for malloc error (terminate shell)
		or wc expansion into multiple files (don't terminate shell)
	*/
int	file_list_check_wc(t_ast *node, t_shell *shell, int code)
{
	t_file	*curr_file;

	if (code == TOKEN_INPUT)
		curr_file = node->input_list;
	else if (code == TOKEN_OUTPUT)
		curr_file = node->output_list;
	while (curr_file)
	{
		if (contain_wc(curr_file->file_name) == 0)
		{
			if (expand_wc_file(shell, node, curr_file->file_name, code))
				return (1);
		}
		curr_file = curr_file->next;
	}
	return (0);
}

/*	redir_check_wc checks input and output list for wildcards 
	- returns 1 if malloc error or system call error */
int	redir_check_wc(t_ast *node, t_shell *shell)
{
	if (node->input_list)
	{
		if (file_list_check_wc(node, shell, TOKEN_INPUT) == 1)
			return (1);
	}
	if (node->output_list)
	{
		if (file_list_check_wc(node, shell, TOKEN_OUTPUT) == 1)
			return (1);
	}
	return (0);
}

/*	redir_check_wc checks input and output list for wildcards 
	- returns 1 if malloc error or system call error */
int	arg_check_wc(t_ast *node, t_shell *shell)
{
	t_list	*curr_arg;

	curr_arg = node->args;
	while (curr_arg)
	{
		if (contain_wc(curr_arg->content) == 0)
		{
			if (expand_wc_arg(shell->directory_contents, node, curr_arg->content) == 1)
				return (1);
		}
		curr_arg = curr_arg->next;
	}
	return (0);
}

/*	check_wc_expansion checks whether wildcard expansions are needed
	- loads and eventually frees shell->directory_contents
	- checks strings in cmd, args and files
	- if needed, expands them
	- returns 1 if malloc error (terminate shell) 
		or wc expanded into multiple files for redirections 
			(don't terminate shell)
	*/
int	check_wc_expansion(t_ast *node, t_shell *shell)
{
	shell->directory_contents = create_conts_list(".");
	if (!shell->directory_contents)
	{
		shell->exit_shell = 1;
		return (1);
	}
	if (contain_wc(node->cmd) == 0)
	{
		if (expand_wc_cmd(shell->directory_contents, node) == 1)
		{
			shell->exit_shell = 1;
			return (free_conts_list(shell->directory_contents), 1);
		}
	}
	if (node->n_args > 0)
	{
		if (arg_check_wc(node, shell) == 1)
		{
			shell->exit_shell = 1;
			return (free_conts_list(shell->directory_contents), 1);
		}
	}
	if (redir_check_wc(node, shell) == 1)
		return (free_conts_list(shell->directory_contents), 1);
	return (free_conts_list(shell->directory_contents), 0);
}
