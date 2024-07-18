/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 06:22:29 by codespace         #+#    #+#             */
/*   Updated: 2024/07/18 16:09:38 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	err_syscall for errors that should terminate shell
	- sets exit_shell to 1
	- prints error message */
int	err_syscall(t_shell *shell, char *cmd)
{
	shell->exit_shell = 1;
	err_printf("error calling %s\n", cmd);
	return (1);
}

/*	err_syntax for syntax errors
	- prints error message with token that is causing error 
	- returns n*/
int	err_syntax(char *token, int n)
{
	err_printf("syntax error near unexpected token `%s'\n", token);
	return (n);
}