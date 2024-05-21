/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:04:03 by mateo             #+#    #+#             */
/*   Updated: 2024/05/21 18:04:17 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	execute_ast executes AST*/
int	execute_ast(t_ast *node)
{
	if (!node)
		return (0);
	if (!node->left && !node->right) 
	{
		// execute command - return 1 or 0
		// return (execute_command(node));

		printf("exe cmd: %s\n", node->cmd);
		return (0);
	}
	else if (node->code == TOKEN_PIPE)
	{
		// ???
	}
	else if (node->code == TOKEN_AND)
	{
		if (execute_ast(node->left) == 0)
			return (execute_ast(node->right));
		else
			return (1);
	}
	else if (node->code == TOKEN_OR)
	{
		if (execute_ast(node->left) == 0)
			return (0);
		else
			return (execute_ast(node->right));
	}
	return (0);
}
