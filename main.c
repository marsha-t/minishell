/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 06:35:42 by mateo             #+#    #+#             */
/*   Updated: 2024/05/13 15:56:06 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(void)
{
	char *line;
	t_token *tokens;

	while(1)
	{
		line = readline("minishell$");
		printf("check_all: %d\n",check_all(line));
		if (check_all(line) != 1) // handle check failures
			return (1);
		tokens = tokenise(line);
		print_tokens(tokens);
		if (!tokens) // handle tokenisation failures
			return (1);
	}
}