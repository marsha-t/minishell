/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:02:50 by mateo             #+#    #+#             */
/*   Updated: 2024/07/16 07:50:23 by mateo            ###   ########.fr       */
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

/*	add_ast_node:
	- calls on _node to malloc space
	- duplicates str in tokens node (to make freeing tokens linked list easier later)
	- moves tokens pointer along by 1
	- attaches new node to end of list
		- current node is included in function to avoid traversal of ast linked list
	- returns 1 if malloc error
*/
int	ast_node_add(t_token **tokens, t_ast **start, t_ast **current)
{
	t_ast	*new;

	new = ast_node_init();
	if (!new)
		return (err_printf("minishell: malloc error: t_ast for ast_node_add\n"), 1);
	new->cmd = ft_strdup((*tokens)->str);
	if (!new->cmd)
		return (err_printf("minishell: malloc error: t_ast->cmd for ast_node_add\n"), 1);
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
	- allocates space for n_args strings
	- duplicates arg strings into array of str in ast node
	- shifts tokens pointer along
	- returns 1 if malloc error for char **args or strdup
		- if strdup error, update n_args so that ast_list_free frees the correct number of strings*/
// work in progress: update description of function
int	ast_node_append_arg(t_token **tokens, t_ast *current)
{
	int	i;
	t_list *new;
	t_list *c;

	i = 0;
	while ((*tokens) && (*tokens)->code == TOKEN_ARG)
	{
		new = malloc(sizeof(t_list));
		if (!new)
			return (err_printf("minishell: malloc error: t_ast for ast_node_append_arg\n"), 1);
		new->content= ft_strdup((*tokens)->str);
		if (!new->content)
			return (err_printf("minishell: malloc error: t_ast->arg for ast_node_append_arg\n"), 1);
		new->next = NULL;
		if(!current->args)
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

/*	ast_node_append_misc adds file to the ast_node
	- tokens pointer is shifted by two: 1) redirection node 2) file node
	- returns 1 if malloc error for strdup*/
int	ast_node_append_misc(t_token **tokens, t_ast **start, t_ast **current)
{
	if (!*start)
	{
		*start = ast_node_init();
		if (!*start)
			return (err_printf("minishell: malloc error: t_ast for ast_node_append_misc\n"), 1);
		*current = *start;
	}
	else if (*start && (*current)->code != TOKEN_CMD)
	{
		(*current)->next = ast_node_init();
		if (!(*current)->next)
			return (err_printf("minishell: malloc error: t_ast for ast_node_append_misc\n"), 1);
		*current = (*current)->next;
	}
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
		if (create_output_append_list((*tokens)->code, (*tokens)->next, current) == 1)
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
		return (err_printf("minishell: malloc error: t_ast->cmd for ast_node_append_cmd\n"), 1);
	current->code = (*tokens)->code;
	*tokens = (*tokens)->next;
	return (0);
}

/*	ast_list_new generates nodes in the ast
	- ast nodes are placed in a linked list
	- frees linked list
*/
t_ast	*ast_list_new(t_token **tokens)
{
	t_ast	*start;
	t_ast	*current;

	start = 0;
	current = 0;
	while (*tokens)
	{
		if ((*tokens)->code == TOKEN_CMD && current && !current->cmd)
		{
			if (ast_node_append_cmd(tokens, current) == 1)
				return (ast_list_free(start), NULL);
		}
		else if ((*tokens)->code == TOKEN_CMD || (*tokens)->code == TOKEN_PIPE || \
		(*tokens)->code == TOKEN_AND || (*tokens)->code == TOKEN_OR || \
		(*tokens)->code == TOKEN_OBRACKET || (*tokens)->code == TOKEN_CBRACKET)
		{
			if (ast_node_add(tokens, &start, &current) == 1)
				return (ast_list_free(start), NULL);
		}
		else if ((*tokens)->code == TOKEN_ARG)
		{
			if (ast_node_append_arg(tokens, current) == 1)
				return (ast_list_free(start), NULL);
		}
		else
		{
			if (ast_node_append_misc(tokens, &start, &current) == 1)
				return (ast_list_free(start), NULL);
		}
	}
	free_tokens(*tokens);
	return (start);
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

/*	file_list_free frees nodes in the files linked list
	and the file names stored in each node */
void	file_list_free(t_file *file)
{
	t_file	*current;
	t_file	*next;

	current = file;
	while (current)
	{
		next = current->next;
		free(current->file_name);
		free(current);
		current = next;
	}
}

void free_pipes_list(t_ast *node)
{
	t_ast	*current;
	current = node;
	while(node)
	{
		dprintf(2,"this is free pipes\n");
		current = node->pipe;
		free(node);
		node = current;
	}
}

/*	ast_list_free frees nodes in ast linked list, cmd,
	args and its component strings, input, output, append */
void	ast_list_free(t_ast *node)
{
	t_ast	*current;
	t_ast	*next;
	t_list	*curr_arg;

	current = node;
	while (current)
	{
		next = current->next;
		if (current->cmd)
			free(current->cmd);
		if (current->args)
		{
			curr_arg = current->args;
			while (curr_arg)
			{
				current->args = current->args->next;
				free(curr_arg->content);
				free(curr_arg);
				curr_arg = current->args;
			}
		}
		if (current->input_list)
			file_list_free(current->input_list);
		if (current->heredoc_list)
			file_list_free(current->heredoc_list);
		if (current->output_list)
			file_list_free(current->output_list);
		free(current);
		current = next;
	}
}
