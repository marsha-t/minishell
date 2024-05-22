/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:27:29 by ryagoub           #+#    #+#             */
/*   Updated: 2024/05/22 20:21:49 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

