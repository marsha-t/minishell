/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 06:35:42 by mateo             #+#    #+#             */
/*   Updated: 2024/06/03 17:51:00 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_shell(t_shell *shell)
{
	free_shell(shell);
	rl_clear_history();
}

int main(int ac, char **av, char **envp)
{
	t_shell	*shell;
	int	exit_status;

	(void)ac;
	(void)av;
	shell = init_shell(envp);
	if (!shell)
		return (1);
	while (1)
	{
		shell->line = readline("minishell$");
		if (shell->line == NULL) // may need to amend for heredoc
			return (exit_shell(shell), 1);
		shell->line = ft_strtrim(shell->line, " \t");
		if (!shell->line)
			return (ft_putstr_fd("Malloc error creating ft_strtrim(shell->line)\n", 2), free_shell(shell), 1);
		if (shell->line[0] == '\0')
			free(shell->line);
		else
		{
			add_history(shell->line);
			if (check_all(shell->line) != 0)
			{
				exit_status = 2;
				free(shell->line);
				continue;
			}
			exit_status = tokenise(shell->line, &shell->tokens);
			if (exit_status == 1)
				return (exit_shell(shell), 1);
			else if (exit_status == 2)
			{
				free(shell->line);
				continue;
			}
			free(shell->line);
			shell->root = parse_tokens(&shell->tokens);
			if (!shell->root)
				return (exit_shell(shell), 1); 
			ast_tree_print(shell->root);
			execute_ast(shell->root, shell);
    	}
	}
	exit_shell(shell);
}
