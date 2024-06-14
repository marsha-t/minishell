/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:27:29 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/14 14:25:17 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char  *expand_var(t_var **env,char *key)
{
	t_var *current;
	current = *env;
	while (current -> next)
	{
		ft_printf("%s this is the key\n", key);
		if(search_for_key(key,current->content) != NULL)
			return(search_for_key(key,current->content));
		current = current -> next;
	}
	return("\0");
}
int key_len(char *str)
{
	int i;

	i = 0;
	while (str[i] != '=' && str[i] != '\0')
	{
		i++;
	}
	return(i);
}
char *return_key(char *str)
{
	int length;
	int i;
	char *key;
	length = key_len(str);
	i = 0;
	key = malloc(sizeof(char) * length);
	while(str[i] != '\0' && str[i] != '=')
	{
		key[i] = str[i];
		i++;
	}
	if(str[i] == '=')
	{
		key[i] = '\0';
		return(key);
	}
	else
		return(NULL);
}

char  *search_for_key(char *key,char *str)
{
	int i;
	char *li_key;

	li_key = return_key(str);
	i = 0;
	if (ft_strcmp(key , li_key) == 0)
	{
		while (str[i] != '=')
			i++;
		i++;
		return(free(li_key), str + i);
	}
	return(free(li_key), NULL);
}

int var_length(char *str)
{
	int i;
	i = 0;
	while(str[i] != '\0')
	{
		if(str[i] == '$')
		{
			i++;
			while(ft_strchr(" \t\"<>()&|", str[i]) == NULL)
			{
				i++;
			}
			return(i + 1);
		}
		i++;
	}
	return(0);
}
// char ret_value(char * envp, char temp ,char str, int *i)
// {

// }
int ft_strlen_b_$(char *str)
{
	int i;
	i = 0;
	while(str[i] != '\0' &&str[i] !=  '$')
	{
		i++;
	}
	if(str[i] == '$')
		return(i);
	return(-1);

}
char *value(char *str , t_var **envp)
{
	char *var;
	char *temp;
	int i;
	int j;

	i = 0;
	j = 0;
	var = malloc(var_length(str) * sizeof(char));
	while(str[i] != '\0')
	{
		if(ft_strlen_b_$(str + i)!= -1)
		{
			j =0;
			temp = malloc(sizeof(char) * ft_strlen_b_$(str + i));
			while(str[i] != '$' && str[i] != '\0')
			{
				temp[j] = str[i];
				i++;
				j++;
			}
		}
		if(str[i] == '$')
		{
			i++;
			j = 0;
			while(ft_strchr(" \t\"<>()&|", str[i]) == NULL)
			{
				var[j] = str[i];
				i++;
				j++;
			}
			var[j] = '\0';
			var = expand_var(envp,var);
			var = ft_strjoin(temp, var);
			 str = ft_strjoin(var, str + i);
			 free(temp);
			 free(var);
		}
		i++;
	}
	return(str);
}

int is_var(char *str)
{
	int i;
	i = 0;
	if(!str)
		return(0);
	if(ft_strlen(str) >= 2)
	{
		while(str[i] != '\0')
		{
				if (str[i] == '$' && (ft_strchr(" \t\"&|<>()",str[i - 1] )))
					return(1);
				i++;
		}
	}
	return(0);
}

