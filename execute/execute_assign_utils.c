/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_assign_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 12:43:47 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/23 13:35:52 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	extract_key_value(char *str, char **equal, char **key, char **value)
{
	*key = strdup_range(str, *equal - 1);
	if (!*key)
		return (err_printf("malloc error: key\n"), 1);
	if (*(*equal + 1) == '\0')
	{
		*value = ft_strdup("");
		if (!*value)
			return (free(*key), err_printf("malloc error: value\n"), 1);
	}
	else
	{
		*value = strdup_range(*equal + 1, str + ft_strlen(str));
		if (!*value)
			return (free(*key), err_printf("malloc error: value\n"), 1);
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
		return (shell->exit_shell = 1, 1);
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
			return (free_num(2, key, value), 1);
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
		if (check_value(equal, key, shell, curr_arg) == 1)
			return (1);
	}
	return (0);
}
