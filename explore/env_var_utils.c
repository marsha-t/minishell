/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:27:29 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/17 06:08:18 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	is_valid_varstart checks whether the char after $ is valid
	- valid if c is a letter or underscore: returns 0 */
int	is_valid_varstart(char c)
{
	if (ft_isalpha(c) == 1)
		return (0);
	if (c == '_')
		return (0);
	else
		return (1);
}

/*	is_valid_varchar returns 0 if char is valid character for a variable
	- valid if c is a letter, number or underscore*/
int	is_valid_varchar(char c)
{
	if (ft_isalpha(c) == 1)
		return (0);
	else if (ft_isdigit(c) == 1)
		return (0);
	else if (c == '_')
		return (0);
	return (1);
}

/*	is_quote returns 0 if c is a single or double quote */
int	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (0);
	return (1);
}

/************************************************************************/
/*	expand_str()														*/
/************************************************************************/
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

char  *expand_var(char *var, t_var *env) // changed to single pointers
{
	t_var *current;
	current = env;
	while (current)
	{
		if(ft_strcmp(current->key,var)==0)
			return(ft_strdup(current -> value));
		current = current -> next;
	}
	// char *n;
	// n= malloc(sizeof(char));
	// n = "\0";
	// return(ft_strdup(n));
	return (ft_strdup(""));
}

/*	join_expand*/
// char *join_expand(int i,char *temp, char *str,char *var)
// {
// 	char *all_str;

// 	all_str= ft_strjoin2(temp,var);
// 	free(temp);
// 	free(var);
// 	temp = all_str;
// 	all_str=ft_strjoin2(temp,str+i);
// 	free(temp);
// 	free(str);
// 	return(all_str);
// }

char *join_expand(char *temp, char *var, char *str, int i)
{
	char *all_str;

	all_str= ft_strjoin2(temp,var);
	free(temp);
	free(var);
	temp = all_str;
	all_str=ft_strjoin2(temp,str+i);
	free(temp);
	free(str);
	return(all_str);
}

char	*split_expand_join(char *str, int i, t_var *list)
{
	char	*temp;
	char	*var;
	int		start;

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
		while (is_valid_varchar(str[i]) == 0)
			i++;
		var = strdup_range(&str[start], &str[i - 1]);
		if (!var)
			return (err_printf("malloc error: var\n"), NULL);
		var = expand_var(var, list);
		if (!var)
			return (err_printf("malloc error: var\n"), NULL);
	}
	else
		var = NULL;
	str = join_expand(temp, var, str, i);
	if (!str)
		return (err_printf("malloc error joining expanded var\n"), NULL);
	return (str);
}

char	*expand_str(char *str, t_var *list)
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
					str = split_expand_join(str, i, list);
					if (!str)
						return (NULL);
					i = -1;
				}
				else
					i++;
			}
			i++;
		}
		else if (str[i] == '$' && is_quote(str[i + 1]) == 0)
		{
			str = split_expand_join(str, i, list);
			if (!str)
				return (NULL);
			i = 0;
		}
		else if (str[i] == '$' && is_valid_varstart(str[i + 1]) == 0)
		{
			str = split_expand_join(str, i, list);
			if (!str)
				return (NULL);
			i = 0;
		}
		else
			i++;
	}
	return (str);
}

// char *expand_str(char *str, t_var *list)
// {
// 	int i;
// 	char *start;
// 	char *temp;
// 	char *var;

// 	 i=0;
// 	 start=NULL;
// 	temp=NULL;
// 	var=NULL;
// 	 while(str[i] != '\0')
// 	 {
// 		if(i == 0 &&str[i] != '$')
// 			start = &str[i];
// 		if (str[i] == '$' && str[i+1] == '\0')
// 			i++;
// 		else if(str[i]== '$'&& str[i+1]!= '\"')
// 		{
// 			if(&str[i] != start && start)
// 			{
// 				temp = strdup_range(start,&str[i]-1);
// 			}
// 			start = &str[++i];
// 			while(ft_strchr("\"\t ()<>$&|",str[i])==NULL)
// 				i++;
// 			var = strdup_range(start,&str[i-1]);
// 			var =expand_var(&var,&list);
// 			str = join_expand(i,temp,str,var);
// 			i=-1;
// 		}
// 		i++;
// 	 }
// 	 return(str);
// }

/************************************************************************/
/*	contain_var()														*/
/************************************************************************/
/*	contain_var checks whether str contains a variable expansion 
	i.e., a $ followed by valid variable name character 
	- if $ is followed by a starting quote, it is counted as a variable expansion (that expands into empty str)
	- if $ followed by number of special character, it doesn't count as variable expansion
	- if $ is inside single quotes, it isn't a variable expansion
	- if there is variable expansion, return (0)
*/
int contain_var(char *str)
{
	int i;

	if (!str)
		return (1);
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == 39)
		{
			i++;
			while (str[i] != '\0' && str[i] != 39)
				i++;
		}
		else if (str[i] == '\"')
		{
			i++;
			while (str[i] != '\0' && str[i] != '\"')
			{
				if (str[i] == '$' && is_valid_varstart(str[i + 1]) == 0)
					return (0);
				i++;
			}
		}
		else if (str[i] == '$' && is_quote(str[i + 1]) == 0)
			return (0);
		else if (str[i] == '$' && is_valid_varstart(str[i + 1]) == 0)
			return (0);
		i++;
	}
	return (1);
}
