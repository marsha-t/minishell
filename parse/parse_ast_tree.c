#include "../minishell.h"

int	is_logical_op(int code)
{
	if (code == TOKEN_OR || code == TOKEN_AND)
		return (1);
	return (0);
}

t_ast	*ast_pipeline_new(t_ast **node)
{
	t_ast	*head;
	t_ast	*left;

	head = NULL;
	left = NULL;

	while (*node)
	{
		if ((*node)->code == TOKEN_PIPE)
			*node = (*node)->next;
		else if ((*node)->code == TOKEN_CMD)
		{
			if (!left)
				left = *node;
			else
			{
				left->pipe = *node;
				left = *node;
			}
			if (!head)
			{
				head = left;
			}
			*node = (*node)->next;
		}
		else if ((*node)->code == TOKEN_OBRACKET)
		{
			*node = (*node)->next;
			if (!left)
				left = (ast_tree_new(node));
			else
			{
				left->pipe = ast_tree_new(node);
				left = left->pipe;
			}
			if (!head)
			{
				head = left;
			}
		}
		else if ((*node)->code == TOKEN_CBRACKET)
		{
			*node = (*node)->next;
		}
		else if (is_logical_op((*node)->code) == 1)
			break;
	}
	return (head);
}

t_ast	*ast_tree_new(t_ast **node)
{
	t_ast	*root;

	if ((*node)->code == TOKEN_CMD)
	{
		if ((*node)->next && (*node)->next->code == TOKEN_PIPE)
			root = ast_pipeline_new(node);
		else
		{
			root = *node;
			*node = (*node)->next;
		}
	}
	else if ((*node)->code == TOKEN_OBRACKET)
	{
		*node = (*node)->next;
		root = ast_tree_new(node);
		if ((*node) && (*node)->code == TOKEN_PIPE)
		{
			root->pipe = ast_pipeline_new(node);
		}
	}
	while (*node)
	{
		if ((*node)->code == TOKEN_CMD)
		{
			if ((*node)->next && (*node)->next->code == TOKEN_PIPE)
			{
				root->right = ast_pipeline_new(node);
			}
			else
			{
				root->right = *node;
				*node = (*node)->next;
			}
		}
		else if ((*node)->code == TOKEN_OBRACKET)
		{
			*node = (*node)->next;
			root->right = ast_tree_new(node);
			if ((*node) && (*node)->code == TOKEN_PIPE)
			{
				root->right->pipe = ast_pipeline_new(node);
				// root = root->right;
			}
		}
		else if (is_logical_op((*node)->code) == 1)
		{
			(*node)->left = root;
			root = *node;
			*node = (*node)->next;
		}
		else if ((*node)->code == TOKEN_CBRACKET)
		{
			*node = (*node)->next;
			return (root);
		}
	}
	return (root);
}

/*	ast_pipeline_print prints the pipes linked list*/
void	ast_pipeline_print(t_ast *node)
{
	int	i;

	i = 1;
	while (node)
	{
		printf("\tpipe %d: %s\n", i, node->cmd);
		i++;
		node = node->pipe;
	}
}
/*	ast_tree_print prints all the nodes in a tree
	- prints node, left, right
	- before going to left and doing the same
	- and then to right and doing the same*/
void	ast_tree_print(t_ast *node)
{
	ft_printf("node: %d, %s\n", node->code, node->cmd);
	if (node->left)
		ft_printf("\tleft: %d, %s\n", node->left->code, node->left->cmd);
	if (node->right)
		ft_printf("\tright: %d, %s\n", node->right->code, node->right->cmd);
	if (node->pipe)
		ast_pipeline_print(node);
	ft_printf("\n\n");
	if (node->left)
		ast_tree_print(node->left);
	if (node->right)
		ast_tree_print(node->right);
}
