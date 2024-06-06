/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 07:43:19 by mateo             #+#    #+#             */
/*   Updated: 2024/06/06 10:47:57 by mateo            ###   ########.fr       */
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
	}
	return (1);
}

/*	builtin_echo runs the echo command
	- error can occur from write errors
	- invalid options are taken as arguments and printed */
int	builtin_echo(t_ast *node, int in_fd, int out_fd)
{
	int	newline;
	int	i;
	
	i = 0;
	newline = 1;
	if (node->n_args == 0 || (node->n_args > 0 && is_newline_arg(node->args[0]) == 1))
	{
		newline = 0;
		i = 1;
	}
	while (i < node->n_args)
	{
		if (write(out_fd, node->args[i], ft_strlen(node->args[i])) == -1);
			return (ft_putstr_fd("Write error for echo\n", 2), 1);
		if (i != node->n_args - 1)
		{
			if (write(out_fd, " ", 1) == -1)
				return (ft_putstr_fd("Write error for echo\n", 2), 1);
		}	
		i++;
	}
	if (newline == 1)
	{
		if (write(out_fd, "\n", 1) == -1)
			return (ft_putstr_fd("Write error for echo\n", 2), 1);
	}
	return (0);
}
