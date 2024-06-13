/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:03:01 by mateo             #+#    #+#             */
/*   Updated: 2024/06/13 13:43:08 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	ft_getcwd uses getcwd to get latest working dir
	- but it also updates size of buffer if getcwd fails due to buffer size 
	- doubles dir size if not enough space initially */
// work in progress - should we set a max limit for dir size e.g., while (size < 10240)?
char	*ft_getcwd(void)
{
	char *dir;
	int	size;
	int	j;
	
	dir = 0;
	size = 1024;
	while (1)
	{
		dir = malloc(sizeof(char) *size);
		if (!dir)
			return (ft_putstr_fd("Malloc error creating dir\n", 2), NULL);
		if (getcwd(dir, size) != NULL)
			break;
		else if (errno == ERANGE)
			size *= 2;
		else
		{
			free(dir);
			return(ft_putstr_fd("Error calling getcwd\n", 2), NULL);
		}
	}
	return (dir);
}

/*	builtin_pwd runs the pwd command
	- error if 
		- options are given
		- malloc error for dir  
		- error from getcwd that are not due to dir size 
	*/
// work in progress: need to terminate shell when error running getcwd
int	builtin_pwd(t_ast *node, int in_fd, int out_fd)
{
	char	*dir;
	int	size;
	
	if (node->n_args > 0 && node->args->content[0] == '-')
		return (ft_putstr_fd("pwd: does not support options", 2), 1);
	dir = ft_getcwd();
	if (!dir)
		return (1); // terminate shell
	write(out_fd, dir, ft_strlen(wd));
	free(dir);
	return (0);
}
