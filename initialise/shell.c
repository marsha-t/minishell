/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:39:16 by mateo             #+#    #+#             */
/*   Updated: 2024/06/03 06:15:14 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_safe(void *pointer)
{
	free(pointer);
	pointer = 0;
}

/*	init_shell initialises t_shell
	- creates linked list of environment variables
	- initialises the rest to 0/NULL */
t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (ft_putstr_fd("Malloc error for shell structure\n", 2), NULL);
	shell->var_list = create_list(envp);
	if (!shell->var_list)
	{
		free(shell);
		ft_putstr_fd("???", 2);
		return (NULL);
	}
	shell->line = 0;
	return (shell);	
}

/*	free_shell frees the various structures within shell */
void	free_shell(t_shell *shell)
{
	free_var_list(shell->var_list);
	free_tokens(shell->tokens);
	// ast_tree_free(shell->root);
}