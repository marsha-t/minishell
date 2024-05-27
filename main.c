/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 06:35:42 by mateo             #+#    #+#             */
/*   Updated: 2024/05/25 18:43:23 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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
		printf("check_all: %d\n",check_all(line));
		// if (check_all(line) != 1) // handle check failures
		// 	return (1);
		tokens = tokenise(line);
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
	free(list);
}
