/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:54:13 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/24 18:02:19 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_exit_status_modif(t_ast *node, t_shell *shell)
{
	shell->exit_status = execute_ast(node->right, shell);
	exit_shell(shell, shell->exit_status);
}
