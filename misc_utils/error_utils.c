/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 06:22:29 by codespace         #+#    #+#             */
/*   Updated: 2024/07/20 19:54:09 by ryagoub          ###   ########.fr       */
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
