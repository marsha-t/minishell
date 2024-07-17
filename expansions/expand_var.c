/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:27:29 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/17 17:53:06 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin2(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	tot_len;
	int		i;

	if (!s1 && s2)
		return (ft_strdup(s2));
	else if (!s2 && s1)
		return (ft_strdup(s1));
	else if (!s1 && !s2)
		return (0);
	tot_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	ptr = (char *)malloc(sizeof(char) * tot_len);
	if (!ptr)
		return (0);
	i = 0;
	while (*s1)
		ptr[i++] = *(s1)++;
	while (*s2)
		ptr[i++] = *(s2)++;
	ptr[i] = '\0';
	return (ptr);
}

/*	expand_var searches for var in variable list 
	- returns duplicate of value if found
	- returns duplicate of empty string if nothing found
	- malloc protection in split_expand_join()*/
char	*expand_var(char *var, t_var *env)
{
	t_var	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, var) == 0)
			return (ft_strdup(current->value));
		current = current -> next;
	}
	return (ft_strdup(""));
}

char	*join_expand(char *temp, char *var, char *str, int i)
{
	char	*all_str;

	all_str = ft_strjoin2(temp, var);
	free(temp);
	free(var);
	temp = all_str;
	all_str = ft_strjoin2(temp, str + i);
	free(temp);
	free(str);
	return (all_str);
}

char	*split_expand_join(char *str, int i, t_shell *shell)
{
	char	*temp;
	char	*var;
	int		start;
	char	*expanded;

	if (i != 0)
	{
		temp = strdup_range(&str[0], &str[i - 1]);
		if (!temp)
			return (err_printf("malloc error: temp\n"), NULL);
	}
	else
		temp = NULL;
	start = i + 1;
	i++;
	if (is_valid_varstart(str[start]) == 0)
	{
		if (str[start] == '?')
		{
			i++;
			var = ft_itoa(shell->exit_status);
			if (!var)
				return (free(temp), err_printf("malloc error: ft_itoa\n"), NULL);
		}
		else
		{
			while (is_valid_varchar(str[i]) == 0)
				i++;
			var = strdup_range(&str[start], &str[i - 1]);
			if (!var)
				return (free(temp), err_printf("malloc error: var\n"), NULL);
			expanded = expand_var(var, shell->var_list);
			printf("expanded: %s\n", expanded);
			free(var);
			var = expanded;
			if (!expanded)
				return (free(temp), err_printf("malloc error: expand_var\n"), NULL);
		}
	}
	else
		var = NULL;
	str = join_expand(temp, var, str, i);
	if (!str)
		return (err_printf("malloc error: str\n"), NULL);
	return (str);
}

char	*expand_str(char *str, t_shell *shell)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == 39)
		{
			i++;
			while (str[i] != '\0' && str[i] != 39)
				i++;
			i++;
		}
		else if (str[i] == '\"')
		{
			i++;
			while (str[i] != '\0' && str[i] != '\"')
			{
				if (str[i] == '$' && is_valid_varstart(str[i + 1]) == 0)
				{
					printf("aa\n");
					str = split_expand_join(str, i, shell);
					printf("bb: %s\n", str);
					if (!str)
						return (NULL);
					i = -1;
				}
				else
					i++;
			}
			printf("i:%d\n", i);
			i++;
		}
		else if (str[i] == '$' && is_quote(str[i + 1]) == 0)
		{
			str = split_expand_join(str, i, shell);
			if (!str)
				return (NULL);
			i = 0;
		}
		else if (str[i] == '$' && is_valid_varstart(str[i + 1]) == 0)
		{
			str = split_expand_join(str, i, shell);
			if (!str)
				return (NULL);
			i = 0;
		}
		else
			i++;
	}
	return (str);
}

/*	file_list_check_var calls contain_var and expand_str
	for each file provided in file linked list
	*/
int	file_list_check_var(t_file *file, t_shell *shell)
{
	t_file	*curr_file;

	curr_file = file;
	while (curr_file)
	{
		if (contain_var(curr_file->file_name) == 0)
		{
			curr_file->file_name = expand_str(curr_file->file_name, shell);
			if (!curr_file->file_name)
				return (1);
		}
		curr_file = curr_file->next;
	}
	return (0);
}

/*	check_var_expansion checks whether variable expansions are needed
	- checks strings in cmd, args and files
	- if needed, expands them
	- returns 1 if errors with expansion (terminate shell)
	*/
int	check_var_expansion(t_ast *node, t_shell *shell)
{
	t_list	*curr_arg;

	if (contain_var(node->cmd) == 0)
	{
		node->cmd = expand_str(node->cmd, shell);
		if (!node->cmd)
			return (1);
	}
	if (node->n_args > 0)
	{
		curr_arg = node->args;
		while (curr_arg)
		{
			if (contain_var(curr_arg->content) == 0)
			{
				printf("a\n");
				curr_arg->content = expand_str(curr_arg->content, shell);
				printf("b\n");

				if (!curr_arg->content)
					return (1);
			}
			curr_arg = curr_arg->next;
		}
	}
	if (node->input_list)
	{
		if (file_list_check_var(node->input_list, shell) == 1)
			return (1);
	}
	if (node->heredoc_list)
	{
		if (file_list_check_var(node->heredoc_list, shell) == 1)
			return (1);
	}
	if (node->output_list)
	{
		if (file_list_check_var(node->output_list, shell) == 1)
			return (1);
	}
	return (0);
}
