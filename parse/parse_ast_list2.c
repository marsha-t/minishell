/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_list2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 18:16:45 by mateo             #+#    #+#             */
/*   Updated: 2024/07/22 18:19:21 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	add_ast_node:
	- calls on ast_node_init to malloc space
	- duplicates str in tokens node 
		(to make freeing tokens linked list easier later)
	- moves tokens pointer along by 1
	- attaches new node to end of list
		- current node is included in function 
			to avoid traversal of ast linked list
	- returns 1 if malloc error
*/
int	ast_node_add(t_token **tokens, t_ast **start, t_ast **current)
{
	t_ast	*new;

	new = ast_node_init();
	if (!new)
		return (err_printf("malloc error: t_ast\n"), 1);
	new->cmd = ft_strdup((*tokens)->str);
	if (!new->cmd)
		return (err_printf("malloc error: t_ast->cmd\n"), 1);
	new->code = (*tokens)->code;
	*tokens = (*tokens)->next;
	if (!(*start))
	{
		*start = new;
		*current = *start;
	}
	else
	{
		(*current)->next = new;
		(*current) = new;
	}
	return (0);
}

/*	ast_node_append_arg appends the arguments into command ast node
	- args are in linked list
	- shifts tokens pointer along
	- returns 1 if malloc error for linked list node or strdup
*/
int	ast_node_append_arg(t_token **tokens, t_ast *current)
{
	int		i;
	t_list	*new;
	t_list	*c;

	i = 0;
	while ((*tokens) && (*tokens)->code == TOKEN_ARG)
	{
		new = create_arg_node((*tokens)->str);
		if (!current->args)
			current->args = new;
		else
		{
			c = current->args;
			while (c->next)
				c = c->next;
			c->next = new;
		}
		current->n_args++;
		*tokens = (*tokens)->next;
		i++;
	}
	return (0);
}

int	ast_node_append_misc_setup(t_ast **start, t_ast **current)
{
	if (!*start)
	{
		*start = ast_node_init();
		if (!*start)
			return (err_printf("malloc error: t_ast\n"), 1);
		*current = *start;
	}
	else if (*start && (*current)->code != TOKEN_CMD && (*current)->code != 0)
	{
		(*current)->next = ast_node_init();
		if (!(*current)->next)
			return (err_printf("malloc error: t_ast\n"), 1);
		*current = (*current)->next;
	}
	return (0);
}

/*	ast_node_append_misc adds file to the ast_node
	- tokens pointer is shifted by two: 1) redirection node 2) file node
	- returns 1 if malloc error for strdup*/
int	ast_node_append_misc(t_token **tokens, t_ast **start, t_ast **current)
{
	if (ast_node_append_misc_setup(start, current) == 1)
		return (1);
	if ((*tokens)->code == TOKEN_INPUT)
	{
		if ((create_in_list((*tokens)->next, current)) == 1)
			return (1);
	}
	else if ((*tokens)->code == TOKEN_HEREDOC)
	{
		if (create_heredoc_list((*tokens)->next, current) == 1)
			return (1);
	}
	else if ((*tokens)->code == TOKEN_OUTPUT || (*tokens)->code == TOKEN_APPEND)
	{
		if (create_output_append_list((*tokens)->code, \
			(*tokens)->next, current) == 1)
			return (1);
	}
	*tokens = (*tokens)->next->next;
	return (0);
}

/*	ast_node_append_cmd appends cmd to an existing ast node
	- used when redirection begins a command
	- returns 1 if malloc error for cmd*/
int	ast_node_append_cmd(t_token **tokens, t_ast *current)
{
	current->cmd = ft_strdup((*tokens)->str);
	if (!current->cmd)
		return (err_printf("malloc error: t_ast->cmd\n"), 1);
	current->code = (*tokens)->code;
	*tokens = (*tokens)->next;
	return (0);
}
