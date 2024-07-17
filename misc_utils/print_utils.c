/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:10:09 by mateo             #+#    #+#             */
/*   Updated: 2024/07/17 16:13:39 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	print_var_list(t_var *var)
{
	t_var	*curr;

	curr = var;
	while (curr)
	{
		if (curr->env == 0)
			ft_printf("%s: %s\n", curr->key, curr->value);
		curr = curr->next;
	}
}

/*	print_token prints info in the token linked list
	- used for debugging */
void	print_tokens(t_token *tokens)
{
	ft_printf("printing tokens: \n");
	if (!tokens)
		ft_printf("empty\n");
	while (tokens)
	{
		ft_printf("str: %s, code: %d\n", tokens->str, tokens->code);
		tokens = tokens->next;
	}
}


/*	file_list_print prints a file linked list
	Depending on the type and file->flag, it will display "input/heredoc/output/append"
	*/
void	file_list_print(t_file *file, int type)
{
	t_file	*current;
	int		i;

	current = file;
	i = 1;
	while (current)
	{
		if (type == 1)
			ft_printf("input file ");
		else if (type == 2)
			ft_printf("heredoc delim ");
		else if (type == 3 && current->flag == TOKEN_OUTPUT)
			ft_printf("output file ");
		else if (type == 3 && current->flag == TOKEN_APPEND)
			ft_printf("append file ");
		ft_printf("%d: %s\n", i, current->file_name);
		current = current->next;
		i++;
	}
}

/*	ast_list_print prints the ast nodes that are connected in a list
	- prints other components in cmd node depending on whether it is filled */
void	ast_list_print(t_ast *node)
{
	t_list *c;

	ft_printf("\nprinting ast_list\n");
	while (node)
	{
		if (node->code == TOKEN_CMD)
		{
			ft_printf("NODE: cmd: %s\nn_args: %d, args: ", node->cmd, node->n_args);
			if (node->args)
			{
				c = node->args;
				while (c)
				{
					ft_printf("%s, ", c->content);
					c = c->next;
				}
			}
			ft_printf("\n");
			if (node->input_list)
				file_list_print(node->input_list, 1);
			if (node->heredoc_list)
				file_list_print(node->heredoc_list, 2);
			if (node->output_list)
				file_list_print(node->output_list, 3);
			ft_printf("\n");
		}
		else
			ft_printf("NODE: %s: %d\n\n", node->cmd, node->code);
		node = node->next;
	}
}

