/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 07:43:19 by mateo             #+#    #+#             */
/*   Updated: 2024/07/01 21:38:17 by ryagoub          ###   ########.fr       */
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
// work in progress: should in_fd do anything?
int	builtin_echo(t_ast *node, int in_fd, int out_fd)
{
	int		newline;
	t_list	*curr_arg;

	(void)in_fd;
	newline = 1;
	if (node->n_args == 0 || (node->n_args > 0 && is_newline_arg(node->args->content) == 1))
		newline = 0;
	curr_arg = node->args;
	while (is_newline_arg(curr_arg->content) == 1)
		curr_arg = curr_arg->next;
	while (curr_arg)
	{
		write(out_fd, curr_arg->content, ft_strlen(curr_arg->content));
		if (curr_arg->next)
			write(out_fd, " ", 1);
		curr_arg = curr_arg->next;
	}
	if (newline == 1)
		write(out_fd, "\n", 1);
	return (0);
}
