/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:59:01 by ryagoub           #+#    #+#             */
/*   Updated: 2024/05/22 09:20:06 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int create_node(t_var **v, char *str)
{
	t_var *new;
	new = (t_var *) malloc(sizeof(t_var));
	t_var *current;
	new = (t_var *) malloc(sizeof(t_var));
	new->content = ft_strdup(str);
	new -> flag = 0;
	if(!new)
		return(0);
	if (!*v)
		*v = new;
	else
	{
		current = *v;
		while(current -> next != NULL)
			current = current->next;
		current -> next = new;
	}
	new -> next = NULL;
	return(1);
}
t_var *create_list(char **envp)
{
	int i;
	t_var *v;
	v = NULL;
	i = 0;
	while(envp[i] != NULL)
	{
		if(create_node(&v, envp[i]) == 0)
			return (NULL);
		i++;
	}
	return(v);
}

char  *expand_var(t_var **env,char *key)
{
	t_var *current;
	current = *env;
	while (current -> next)
	{
		printf("%s this is the key\n", key);
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
