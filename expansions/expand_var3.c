/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:10:02 by mateo             #+#    #+#             */
/*   Updated: 2024/07/23 15:10:53 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

/*	join_expand joins the expanded var together to reform the str */
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

/*	split_expand_join_temp is a helper function to split_expand_join
	- generates temp
	- returns 0 if successful */
int	split_expand_join_temp(char *str, int i, char **temp)
{
	if (i != 0)
	{
		*temp = strdup_range(&str[0], &str[i - 1]);
		if (!*temp)
			return (err_printf("malloc error: temp\n"), 1);
	}
	else
		*temp = NULL;
	return (0);
}

/*	split_expand_join_var is a helper function to split_expand_join
	- returns var */
char	*split_expand_join_var(char *str, int start, int *i, t_shell *shell)
{
	char	*expanded;
	char	*var;

	if (str[start] == '?')
	{
		(*i)++;
		var = ft_itoa(shell->exit_status);
		if (!var)
			return (err_printf("malloc error: ft_itoa\n"), NULL);
	}
	else
	{
		while (is_valid_varchar(str[*i]) == 0)
			(*i)++;
		var = strdup_range(&str[start], &str[*i - 1]);
		if (!var)
			return (err_printf("malloc error: var\n"), NULL);
		expanded = expand_var(var, shell->var_list);
		free(var);
		var = expanded;
		if (!expanded)
			return (err_printf("malloc error: expand_var\n"), NULL);
	}
	return (var);
}

/*	split_expand_join
	- splits the str into temp (before $), var and the rest
	- expands var
	- joins everything back together
*/
char	*split_expand_join(char *str, int i, t_shell *shell)
{
	char	*temp;
	char	*var;
	int		start;

	if (split_expand_join_temp(str, i, &temp) == 1)
		return (NULL);
	start = i + 1;
	i++;
	if (is_valid_varstart(str[start]) == 0)
	{
		var = split_expand_join_var(str, start, &i, shell);
		if (!var)
			return (free(temp), NULL);
	}
	else
		var = NULL;
	str = join_expand(temp, var, str, i);
	if (!str)
		return (err_printf("malloc error: str\n"), NULL);
	return (str);
}
