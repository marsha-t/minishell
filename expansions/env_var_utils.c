/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:27:29 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/10 21:15:37 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin2(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	tot_len;
	int		i;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
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
char  *expand_var(char **var, t_var **env)
{
	t_var *current;
	current = *env;
	while (current)
	{
		if(ft_strcmp(current->key,*var)==0)
			return(ft_strdup(current -> value));
		current = current -> next;
	}
	char *n;
	n= malloc(sizeof(char));
	n = "\0";
	return(ft_strdup(n));
}
char *join_expand(int i,char *temp, char *str,char *var)
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
// char *strdup_range(char *start, char *end)
// {
// 	char *ptr;
// 	size_t	n;
// 	int i;

// 	n = end - start + 1;
// 	ptr = malloc(sizeof(char) * (n + 1));
// 	printf("n + 1: %ld\n", n + 1);
// 	if (!ptr)
// 		return (0);
// 	i = 0;
// 	while (start != end)
// 	{
// 		ptr[i] = *start;
// 		start++;
// 		printf("%c\n", ptr[i]);
// 		i++;
// 	}
// 	ptr[i] = *end;
// 	printf("%c\n", ptr[i]);
// 	ptr[i+1] = '\0';
// 	printf("i+1: %d\n", i+1);
// 	return (ptr);
// }
char *expand_str(char *str, t_var *list)
{
	int i;
	char *start;
	char *temp;
	char *var;

	 i=0;
	 start=NULL;
	temp=NULL;
	var=NULL;
	printf("%sHERE IS\n",str);
	 while(str[i] != '\0')
	 {
		if(i == 0 &&str[i] != '$')
			start = &str[i];
		else if(str[i]== '$'&& str[i+1]!= '\"')
		{
			if(&str[i] != start && start)
			{
				temp = strdup_range(start,&str[i]-1);
			}
			start = &str[++i];
			while(ft_strchr("\"\t ()<>$&|",str[i])==NULL)
				i++;
			var = strdup_range(start,&str[i-1]);
			var =expand_var(&var,&list);
			str = join_expand(i,temp,str,var);
			i=-1;
		}
		i++;
	 }
	 return(str);
}
// int key_len(char *str)
// {
// 	int i;

// 	i = 0;
// 	while (str[i] != '=' && str[i] != '\0')
// 	{
// 		i++;
// 	}
// 	return(i);
// }
// char *return_key(char *str)
// {
// 	int length;
// 	int i;
// 	char *key;
// 	length = key_len(str);
// 	i = 0;
// 	key = malloc(sizeof(char) * length);
// 	while(str[i] != '\0' && str[i] != '=')
// 	{
// 		key[i] = str[i];
// 		i++;
// 	}
// 	if(str[i] == '=')
// 	{
// 		key[i] = '\0';
// 		return(key);
// 	}
// 	else
// 		return(NULL);
// }

// char  *search_for_key(char *key,char *str)
// {
// 	int i;
// 	char *li_key;

// 	li_key = return_key(str);
// 	i = 0;
// 	if (ft_strcmp(key , li_key) == 0)
// 	{
// 		while (str[i] != '=')
// 			i++;
// 		i++;
// 		return(free(li_key), str + i);
// 	}
// 	return(free(li_key), NULL);
// }

// int var_length(char *str)
// {
// 	int i;
// 	i = 0;
// 	while(str[i] != '\0')
// 	{
// 		if(str[i] == '$')
// 		{
// 			i++;
// 			while(ft_strchr(" \t\"<>()&|", str[i]) == NULL)
// 			{
// 				i++;
// 			}
// 			return(i + 1);
// 		}
// 		i++;
// 	}
// 	return(0);
// }
// char ret_value(char * envp, char temp ,char str, int *i)
// {

// }
// int ft_strlen_b_$(char *str)
// {
// 	int i;
// 	i = 0;
// 	while(str[i] != '\0' &&str[i] !=  '$')
// 	{
// 		i++;
// 	}
// 	if(str[i] == '$')
// 		return(i);
// 	return(-1);

// }
// char *value(char *str , t_var **envp)
// {
// 	char *var;
// 	char *temp;
// 	int i;
// 	int j;

// 	i = 0;
// 	j = 0;
// 	var = malloc(var_length(str) * sizeof(char));
// 	while(str[i] != '\0')
// 	{
// 		if(ft_strlen_b_$(str + i)!= -1)
// 		{
// 			j =0;
// 			temp = malloc(sizeof(char) * ft_strlen_b_$(str + i));
// 			while(str[i] != '$' && str[i] != '\0')
// 			{
// 				temp[j] = str[i];
// 				i++;
// 				j++;
// 			}
// 		}
// 		if(str[i] == '$')
// 		{
// 			i++;
// 			j = 0;
// 			while(ft_strchr(" \t\"<>()&|", str[i]) == NULL)
// 			{
// 				var[j] = str[i];
// 				i++;
// 				j++;
// 			}
// 			var[j] = '\0';
// 			var = expand_var(envp,var);
// 			var = ft_strjoin(temp, var);
// 			 str = ft_strjoin(var, str + i);
// 			 free(temp);
// 			 free(var);
// 		}
// 		i++;
// 	}
// 	return(str);
// }
// contain var will check if there is $ (for expansion) in the token and will return 0 (success) and 1 otherwise
// }
int contain_var(char *str)
{
	int i;
	int flag;
	flag = 0;
	i = 0;
	if(!str)
		return(1);
	if(ft_strlen(str) >= 2)
	{
		while(str[i] != '\0')
		{
				if(str[i] == '$' && str[i + 1] == '\"' && str[i + 2] != '\0')
					str[i++] = ' ';
				if (str[i] == '$' && str[i + 1] == '\"' && str[i + 2] == '\0')
					i++;
				if (str[i] == '$' && (ft_strchr(" \t&|<>()",str[i + 1] )== NULL))
					flag = 1;
				if (str[i] == '*')
				{
					while (str[i] == '*')
						i++;
					return(2);
				}
				// printf("%c",str[i]);
				i++;
		}
	}
	if(flag == 1)
		return(0);
	return(1);
}

