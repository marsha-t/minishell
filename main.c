/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 06:35:42 by mateo             #+#    #+#             */
/*   Updated: 2024/06/16 21:39:22 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int main(int ac, char **av, char **envp)
{
  // t_ast	*root;
	// t_token *token;
	t_shell	*shell;

	(void)ac;
	(void)av;
	shell = init_shell(envp);
	if (!shell)
		return (1);
	while (1)
	{
		shell->line = readline("minishell$");
		if (shell->line == NULL) // to amend to cater for NULL string
			return(1);
		shell->line = ft_strtrim(shell->line, " \t");

		if (shell->line[0] == '\0')
			free(shell->line);
		else
		{
			add_history(shell->line);
			ft_printf("check_all: %d\n",check_all(shell->line));
			// if (check_all(line) != 1) // handle check failures
			// 	return (1);
			shell->tokens = tokenise(shell->line);
			free(shell->line);
			if (!shell->tokens) // handle tokenisation failures
				return (1);
			print_tokens(shell->tokens);
			// t_token *token;
			// token = shell->tokens;
			// while (token)
			// {
			// 	if(contain_var(token->str) == 0)
			// 		token->str =	expand_str(token->str,shell->var_list);
			// 	token  = token->next;
			// }
			shell->root = parse_tokens(&shell->tokens);
      		ft_printf("\nprinting ast_tree\n");
			ast_tree_print(shell->root);
			// execute_ast(shell->root);
    }
	}
	free_shell(shell);
	// rl_clear_history();
}
