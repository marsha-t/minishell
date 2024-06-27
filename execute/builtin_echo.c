/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 07:43:19 by mateo             #+#    #+#             */
/*   Updated: 2024/06/19 12:43:01 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	is_newline_arg returns 1 if arg is a newline argument for echo
	"-n" and "-nnnnnn" are newline arguments */
int	is_newline_arg(char *arg)
{
	int i;

	if (arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/*	builtin_echo runs the echo command
	- error can occur from write errors
	- multiple newline arguments are all considered as newline arguments
	- invalid options are taken as arguments and printed */
int	builtin_echo(t_ast *node)
{
	int		newline;
	t_list	*curr_arg;
	
	newline = 1;
	if (node->n_args > 0 && is_newline_arg(node->args->content) == 1)
		newline = 0;
	if (node->n_args > 0)
	{
		curr_arg = node->args;
		while (is_newline_arg(curr_arg->content) == 1)
			curr_arg = curr_arg->next;
		while (curr_arg)
		{
			write(STDOUT_FILENO, curr_arg->content, ft_strlen(curr_arg->content));
			if (curr_arg->next)
				write(STDOUT_FILENO, " ", 1);
			curr_arg = curr_arg->next;
		}
	}
	if (newline == 1)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
