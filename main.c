/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 06:35:42 by mateo             #+#    #+#             */
/*   Updated: 2024/05/28 14:07:22 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	init_shell initialises t_shell
	- creates linked list of environment variables
	- initialises the rest to 0/NULL */
int	init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (1);
	
	// create list for environment var
	// create list for normal var
	shell->var_list = 0;
	shell->line = 0;
	return (0);	
}

int main(int ac, char **av, char **envp)
{
	char *line;
	t_token *tokens;
	// t_ast	*root;
	(void)ac;
	(void)av;
	t_token *token;
	t_var **list;

	list = malloc(sizeof(t_var*));
	*list = create_list(envp);
	while(1)
	{
		line = readline("minishell$");
		if (line == NULL) // to amend to cater for NULL string
			return(1);
		line = ft_strtrim(line, " \t");
		if (line[0] == '\0')
			free(line);
		else
		{
			add_history(line);
			printf("check_all: %d\n",check_all(line));
			// if (check_all(line) != 1) // handle check failures
			// 	return (1);
			tokens = tokenise(line);
			free(line);
			// int size = num_of_tokens(token);
			token = tokens;
			// environment variables operations
			env_ops(list, token);
			print_tokens(tokens);
			if (!tokens) // handle tokenisation failures
				return (1);
			print_tokens(tokens);
			// root = parse_tokens(&tokens);
			// ast_tree_print(root);
			// execute_ast(root);
		}
	}
	free(list);
	rl_clear_history();
}
