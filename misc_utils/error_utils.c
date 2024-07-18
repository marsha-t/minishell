/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 06:22:29 by codespace         #+#    #+#             */
/*   Updated: 2024/07/18 10:49:40 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int    err_syscall(t_shell *shell, char *cmd)
{
    shell->exit_shell = 1;
    err_printf("error calling %s\n", cmd);
    return (1);
}
