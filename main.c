/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 06:35:42 by mateo             #+#    #+#             */
/*   Updated: 2024/05/25 18:43:23 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(void)
{
	char *line;
	t_token *tokens;
	// t_ast	*root;

	while(1)
	{
		line = readline("minishell$");
		printf("check_all: %d\n",check_all(line));
		// if (check_all(line) != 1) // handle check failures
		// 	return (1);
		tokens = tokenise(line);
		if (!tokens) // handle tokenisation failures
			return (1);
		print_tokens(tokens);
		// root = parse_tokens(&tokens);
		// ast_tree_print(root);
		// (void) root;
		// execute_ast(root);
	}
}
