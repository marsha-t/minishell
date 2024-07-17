/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_assign.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 08:33:35 by mateo             #+#    #+#             */
/*   Updated: 2024/07/17 14:25:02 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	valid_varname checks whether a name is valid
	- only contains letters, digits or _
	- cannot start with a digit
	- returns 0 if name is valid; 1 otherwise */
int	valid_varname(char *name)
{
	int	i;

	i = 0;
	if (name == NULL)
		return (1);
	while (name[i])
	{
		if (ft_isalnum(name[i]) == 1 || name[i] == '_')
			i++;
		else
			return (1);
	}
	if (ft_isdigit(name[0]) == 1)
		return (1);
	return (0);
}

/*	create_key_value splits str into key, equal and value */
int	create_key_value(char *str, char **equal, char **key, char **value)
{
	*equal = ft_strchr(str, '=');
	if (*equal)
	{
		if (*equal == str)
		{
			*key = 0;
			*value = 0;
		}
		else
		{
			*key = strdup_range(str, *equal - 1);
			if (!*key)
				return (err_printf("malloc error: key\n"), 1);
			if (*(*equal + 1) == '\0')
			{
				*value = ft_strdup("");
				if (!*value)
				{
					free(*key);
					return (err_printf("malloc error: value\n"), 1);
				}
			}
			else
			{
				*value = strdup_range(*equal + 1, str + ft_strlen(str));
				if (!*value)
				{
					free(*key);
					return (err_printf("malloc error: value\n"), 1);
				}
			}
		}
	}
	else
	{
		*key = ft_strdup(str);
		if (!*key)
			return (err_printf("malloc error: key\n"), 1);
		*value = 0;
	}
	return (0);
}

/*	create_node_normal creates a node for a normal variable
	(i.e., env = 0)
	and adds it to the end of shell->var_list*/
int	create_node_normal(t_var **v, char *key, char *value)
{
	t_var	*new;
	t_var	*current;

	new = malloc(sizeof(t_var));
	if (!new)
		return (err_printf("malloc error: t_var\n"), 1);
	new->key = key;
	new->value = value;
	new->env = 0;
	new->flag = 0;
	new->next = NULL;
	if (!*v)
		*v = new;
	else
	{
		current = *v;
		while (current->next != NULL)
			current = current->next;
		current->next = new;
	}
	return (0);
}

/*	run_assign_str runs the assignment given (either in cmd or arg)
	- if key name already exists, replace value
	- else checks if key name is valid
		if yes, new key added to end of linked list
		if not, error msg */
// work in progress: is valid_varname checked twice?
int	run_assign_str(char *cmd, t_shell *shell)
{
	char	*equal;
	char	*key;
	char	*value;
	t_var	*exist;

	if (create_key_value(cmd, &equal, &key, &value) == 1)
	{
		shell->exit_shell = 1;
		return (1);
	}
	if (valid_varname(key) == 1)
	{
		free_num(2, key, value);
		return (err_printf("%s: command not found\n", cmd), 1);
	}
	exist = check_exist(key, shell->var_list);
	if (exist)
	{
		free(exist->value);
		exist->value = value;
		free(key);
	}
	else
	{
		if (create_node_normal(&shell->var_list, key, value) == 1)
		{
			free_num(2, key, value);
			return (1);
		}
	}
	return (0);
}

/*	check_assign_varname checks that key in assignments is valid
	- checks cmd and args (only args that have the equal sign) */
int	check_assign_varname(t_ast *node, t_shell *shell)
{
	char	*equal;
	char	*key;
	t_list	*curr_arg;

	equal = ft_strchr(node->cmd, '=');
	key = strdup_range(node->cmd, equal - 1);
	if (!key)
	{
		shell->exit_shell = 1;
		return (err_printf("malloc error: key for check_assign_varname\n"), 1);
	}
	if (valid_varname(key) == 1)
		return (err_printf("%s: command not found\n", node->cmd), 1);
	free(key);
	curr_arg = node->args;
	if (node->n_args > 0)
	{
		while (curr_arg)
		{
			equal = ft_strchr(curr_arg->content, '=');
			if (equal)
			{
				key = strdup_range(curr_arg->content, equal - 1);
				if (!key)
				{
					shell->exit_shell = 1;
					return (err_printf("malloc error: key for check_assign_varname\n"), 1);
				}
				if (valid_varname(key) == 1)
					return (err_printf("%s: command not found\n", curr_arg->content), 1);
				free(key);
			}
			else
				break ;
			curr_arg = curr_arg->next;
		}
	}
	return (0);
}

/*	run_assign_cmd runs the remaining args in assignment as a simple command
	- replace original cmd and args with non-assignment args
	- execute updated cmd node using execute_cmd
*/
// work in progress since it uses execute_cmd_node() which is incomplete
// it uses execute_cmd_node because it needs to do cmd_only_quote check
// expansions and redirections are not required to 'redo'
// can consider creating another function instead of execute_cmd_node()

int	run_assign_cmd(t_ast *node, t_shell *shell)
{
	t_list	*arg;

	free(node->cmd);
	node->cmd = node->args->content;
	arg = node->args;
	node->args = node->args->next;
	free(arg);
	node->n_args = ft_lstsize(node->args);
	return (execute_cmd_node(node, shell));
}

/*	run_assign runs the assignments in cmd and args (if any)
	- run_assign_str adds variables into shell->var_list
		- each arg node is freed after each addition into var_list
	- returns 1 if failure (malloc) 
	- if there are remaining args after assignment, ast node is configured 
		so first arg is a cmd
	- otherwise, return 0 */
int	run_assign(t_ast *node, t_shell *shell)
{
	t_list	*curr_arg;

	if (check_assign_varname(node, shell) == 1)
		return (1);
	if (run_assign_str(node->cmd, shell) == 1)
		return (1);
	if (node->n_args > 0)
	{
		curr_arg = node->args;
		while (curr_arg)
		{
			if (ft_strchr(curr_arg->content, '='))
			{
				if (run_assign_str(curr_arg->content, shell) == 1)
					return (1);
				node->args = curr_arg->next;
				free(curr_arg->content);
				free(curr_arg);
				curr_arg = node->args;
			}
			else
				break ;
		}
		if (curr_arg)
		{
			return (run_assign_cmd(node, shell));
		}
	}
	return (0);
}
