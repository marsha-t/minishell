/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:58:00 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/23 18:43:17 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prep_readline(t_shell *shell)
{
	int	i;

	shell->line = readline("minishell$");
	if (shell->line == NULL)
		return (exit_shell(shell, 0), 1);
	i = 0;
	while (shell->line[i] == ' ' || shell->line[i] == '\t')
		i++;
	if (shell->line[i] != '\0')
		add_history(shell->line);
	shell->line = ft_strtrim(shell->line, " \t");
	if (!shell->line)
		return (err_printf("malloc error: ft_strtrim\n"), free_shell(shell), 1);
	return (0);
}

int	run_command(t_shell *shell)
{
	int	tok_status;

	if (check_all(shell->line) != 0)
	{
		shell->exit_status = 2;
		free(shell->line);
		return (0);
	}
	tok_status = tokenise(shell->line, &shell->tokens);
	if (tok_status == 1)
		return (exit_shell(shell, 1), 1);
	else if (tok_status == 2)
	{
		shell->exit_status = 2;
		free_after_command(shell);
		return (0);
	}
	if (parse_tokens(shell) == 1)
		return (exit_shell(shell, 1), 1);
	execute_ast(shell->root, shell);
	free_after_command(shell);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;

	shell = init_shell(ac, av, envp);
	if (!shell)
		return (1);
	while (1)
	{
		if (prep_readline(shell) == 1)
			return (1);
		if (shell->line[0] == '\0')
			free(shell->line);
		else
		{
			if (run_command(shell) == 1)
				return (1);
		}
	}
	exit_shell(shell, 0);
}
