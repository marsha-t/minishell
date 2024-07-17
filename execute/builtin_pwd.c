/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:03:01 by mateo             #+#    #+#             */
/*   Updated: 2024/07/17 06:06:54 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	update_pwd updates PWD in environment variables 
	with a copy of dir (from getcwd())
	- if PWD doesn't exist, it returns with success 
	- note that dir is duplicated here*/
int	update_pwd(char *dir, t_shell *shell)
{
	t_var	*pwd_node;

	pwd_node = check_exist("PWD", shell->var_list);
	if (!pwd_node)
		return (0);
	pwd_node->value = ft_strdup(dir);
	if (!pwd_node->value)
	{
		shell->exit_shell = 1;
		return (err_printf("malloc error: t_var->value\n"), 1);
	}
	return (0);
}

/*	ft_getcwd uses getcwd to get latest working dir
	- but it also updates size of buffer if getcwd fails due to buffer size 
	- doubles dir size if not enough space initially 
	- updates PWD*/
// work in progress - should we set a max limit for dir size e.g., while (size < 10240)?
char	*ft_getcwd(t_shell *shell)
{
	char	*dir;
	int		size;
	
	dir = 0;
	size = 1024;
	while (1)
	{
		dir = malloc(sizeof(char) * size);
		if (!dir)
		{
			shell->exit_shell = 1;
			return (err_printf("malloc error: dir\n"), NULL);
		}
		if (getcwd(dir, size) != NULL)
			break ;
		else if (errno == ERANGE)
		{
			free(dir);
			size *= 2;
		}
		else
		{
			free(dir);
			shell->exit_shell = 1;
			return (err_printf("error calling getcwd\n"), NULL);
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
int	builtin_pwd(t_ast *node, t_shell *shell)
{
	char	*dir;

	if (node->n_args > 0 && ft_strncmp(node->args->content, "-", 1))
		return (err_printf("pwd: does not support options"), 1);
	dir = ft_getcwd(shell);
	if (!dir)
	{
		shell->exit_shell = 1;
		return (1);
	}
	write(STDOUT_FILENO, dir, ft_strlen(dir));
	write(STDOUT_FILENO, "\n", 1);
	free(dir);
	return (0);
}
