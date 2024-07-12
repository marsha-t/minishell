/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:58:00 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/10 10:01:41 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "minishell.h"

int main(int ac, char **av, char **envp)
{
	t_shell	*shell;
	int	tok_status;

	(void)ac;
	(void)av;
	shell = init_shell(envp);
	control_signals();
	if (!shell)
		return (1);
	while (1)
	{
		shell->line = readline("minishell$");
		if (shell->line == NULL)
			return (exit_shell(shell, 1), 1);
		if (shell->line[0] != ' ' && shell->line[0] != '\t' && shell->line[0] != '\0')
			add_history(shell->line);
		shell->line = ft_strtrim(shell->line, " \t");
		if (!shell->line)
			return (err_printf("minishell: malloc error: ft_strtrim(shell->line)\n"), free_shell(shell), 1);
		if (shell->line[0] == '\0')
			free(shell->line);
		else
		{
			if (check_all(shell->line) != 0)
			{
				tok_status = 2;
				free(shell->line);
				continue ;
			}
			tok_status = tokenise(shell->line, &shell->tokens);
			if (tok_status == 1)
				return (exit_shell(shell, 1), 1);
			else if (tok_status == 2)
			{
				free(shell->line);
				continue;
			}
			// free(shell->line);
			if (parse_tokens(shell) == 1)
				return (exit_shell(shell, 1), 1);
			// ast_tree_print(shell->root);
			printf("before execute\n");
			execute_ast(shell->root, shell);
			free_after_command(shell);
    	}
	}
	exit_shell(shell, 0);
}
