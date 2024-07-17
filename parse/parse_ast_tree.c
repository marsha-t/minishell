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
			break;
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
