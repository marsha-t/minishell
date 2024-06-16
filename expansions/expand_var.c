/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:27:29 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/16 21:59:04 by mateo            ###   ########.fr       */
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
			return (ft_putstr_fd("Malloc error creating temp\n", 2), NULL);
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
			return (ft_putstr_fd("Malloc error creating var\n", 2), NULL);
		var = expand_var(var, list);
		if (!var)
			return (ft_putstr_fd("Malloc error creating var\n", 2), NULL);
	}
	else
		var = NULL;
	str = join_expand(temp, var, str, i);
	if (!str)
		return (ft_putstr_fd("Malloc error joining expanded var\n", 2), NULL);
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

