/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:58:00 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/18 11:26:24 by mateo            ###   ########.fr       */
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
		int i;
		i  = 0;
		while(shell->line[i] == ' ' || shell->line[i] == '\t')
			i++;
		if (shell->line[i] != '\0')
			add_history(shell->line);
		shell->line = ft_strtrim(shell->line, " \t");
		if (!shell->line)
			return (err_printf("malloc error: ft_strtrim(shell->line)\n"), free_shell(shell), 1);
		if (shell->line[0] == '\0')
			free(shell->line);
		else
		{
			if (check_all(shell->line) != 0)
			{
				shell->exit_status = 2;
				free(shell->line);
				continue ;
			}
			tok_status = tokenise(shell->line, &shell->tokens);
			if (tok_status == 1)
				return (exit_shell(shell, 1), 1);
			else if (tok_status == 2)
			{
				shell->exit_status = 2;
				free_after_command(shell);
				continue ;
			}
			if (parse_tokens(shell) == 1)
				return (exit_shell(shell, 1), 1);
			ft_printf("execute: \n");
			execute_ast(shell->root, shell);
			free_after_command(shell);
    	}
	}
	exit_shell(shell, 0);
}
