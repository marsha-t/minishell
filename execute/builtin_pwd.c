/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:03:01 by mateo             #+#    #+#             */
/*   Updated: 2024/06/06 10:51:22 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	builtin_pwd runs the pwd command
	- error if 
		- options are given
		- malloc error for dir 
		- error from getcwd that are not due to dir size
		- error from write
	- doubles dir size if not enough space initially */
// work in progress - should we set a max limit for dir size e.g., while (size < 10240)?
int	builtin_pwd(t_ast *node, int in_fd, int out_fd)
{
	char	*dir;
	int	size;
	
	if (node->n_args > 0 && node->args[0][0] == '-')
		return (ft_putstr_fd("pwd: does not support options", 2), 1);
	dir = 0;
	size = 1024;
	while (1)
	{
		dir = malloc(sizeof(char) * size);
		if (!dir)
			return (ft_putstr_fd("Malloc error creating dir\n", 2), 1);
		if (getcwd(wd, size) != NULL)
		{
			if (write(out_fd, wd, ft_strlen(wd)) == -1)
			{
				// error message: write error
				return (1);
			}
			else
				break;
		}
		else
		{
			if (errno == ERANGE)
				size *= 2;
			else
			{
				// error msg: other error from getcwd
				free(dir);
				return (1);
			}
		}
		free(dir);
	}
	return (0);
}
