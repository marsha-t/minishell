/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_list_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 18:18:31 by mateo             #+#    #+#             */
/*   Updated: 2024/07/22 18:18:56 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	ast_node_init allocates space for ast node and
	initialises everything to 0/NULL */
t_ast	*ast_node_init(void)
{
	t_ast	*new;

	new = malloc(sizeof(t_ast));
	if (!new)
		return (NULL);
	new->cmd = 0;
	new->code = 0;
	new->n_args = 0;
	new->args = 0;
	new->input_list = 0;
	new->output_list = 0;
	new->heredoc_list = 0;
	new->root = 0;
	new->next = 0;
	new->left = 0;
	new->right = 0;
	new ->tmp_stdin_fd = 0;
	new ->tmp_stdout_fd = 1;
	new->pipe = 0;
	return (new);
}

t_list	*create_arg_node(char *str)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (err_printf("malloc error: t_ast\n"), NULL);
	new->content = ft_strdup(str);
	if (!new->content)
		return (free(new), err_printf("malloc error: t_ast->arg\n"), NULL);
	new->next = NULL;
	return (new);
}
