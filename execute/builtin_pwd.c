/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:03:01 by mateo             #+#    #+#             */
/*   Updated: 2024/06/17 01:58:40 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	update_pwd updates PWD in environment variables 
	with dir (from getcwd())*/
// work in progress: complete function 
int	update_pwd(char *dir, t_shell *shell)
{
	
}

/*	ft_getcwd uses getcwd to get latest working dir
	- but it also updates size of buffer if getcwd fails due to buffer size 
	- doubles dir size if not enough space initially 
	- updates PWD*/
// work in progress - should we set a max limit for dir size e.g., while (size < 10240)?
// work in progress - update PWD in environment variables every time getcwd is called
char	*ft_getcwd(t_shell *shell)
{
	char *dir;
	int	size;
	
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
	if (update_pwd(dir, shell) == 1)
		return (NULL);
	return (dir);
}

/*	builtin_pwd runs the pwd command
	- error if 
		- options are given: i.e. start with -
		- malloc error for dir  
		- error from getcwd that are not due to dir size 
	*/
// work in progress: need to terminate shell when error running getcwd
// work in progress: check whether this function needs in_fd?
int	builtin_pwd(t_ast *node, int in_fd, int out_fd, t_shell *shell)
{
	char	*dir;
	
	(void)in_fd;
	if (node->n_args > 0 && ft_strncmp(node->args->content, "-", 1))
		return (ft_putstr_fd("pwd: does not support options", 2), 1);
	dir = ft_getcwd(shell);
	if (!dir)
		return (1); // terminate shell
	write(out_fd, dir, ft_strlen(dir));
	write(out_fd, "\n", 1);
	free(dir);
	return (0);
}
