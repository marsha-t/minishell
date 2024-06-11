/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:01:19 by codespace         #+#    #+#             */
/*   Updated: 2024/06/11 21:45:36 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void create_conts_node(char *str, t_dconts **list)
{
    t_dconts *curr;
    t_dconts *new;

    curr = *list;
    new = ft_calloc(1,sizeof(t_dconts));
	if(!str)
		return ;
    if(!new)
        return ft_putstr_fd("malloc error :create directory_contents\n", 2) ;
    new->cont_name = ft_strdup(str);
    new -> next = NULL;
    if(!curr)
    {
        curr = new;
        return ;
    }
    while(curr->next)
        curr = curr->next;
    curr->next = new;
}

t_dconts *create_conts_list(void)
{
    struct dirent *content;
    DIR *dd;
    t_dconts *list;

    dd= opendir(".");
    list = ft_calloc(1,sizeof(t_dconts));
    if(!list)
     return (ft_putstr_fd("malloc error :create directory_contents\n", 2), NULL );
    content = readdir(dd);
    while (content)
    {
        create_conts_node(content -> d_name,&list);
        content = readdir(dd);
    }
    return(list);
}
char *word_suff(char *str, char *word_e)
{
	char *suff;
	int i;
	if(!str|| !word_e)
		return(NULL);
	suff= malloc(sizeof(char) *(ft_strlen(word_e)+1));
	int l1;
	int l2;
	if(!str|| !word_e)
		return(NULL);
	l1 = ft_strlen(str)-1;
	l2 = ft_strlen(word_e)-1;
	i =0;
	while(l1 && l2 >= 0)
	{
		suff[i]= str[l1];
		l1--;
		l2--;
		i++;
	}
	suff[i] = '\0';
	return(ft_strrev(suff));
}
char *word_pref(char *str, char *word_b)
{
	int i;
	char *pref;

	i =0;
	if(!str || !word_b)
		return(NULL);
	pref = malloc(sizeof(char)*(ft_strlen(word_b)+ 1));
	while( word_b[i]!= '\0' && str[i] != '\0')
	{
		pref[i] =str[i];
		i++;
	}
	pref[i] = '\0';
	return(pref);
}
t_dconts *goal_list(t_dconts *conts_list,char *word_b,char *word_e)
{
	t_dconts *curr;
	t_dconts *goal_list;

	curr = conts_list;
	goal_list = ft_calloc(1,sizeof(t_dconts));
	while(curr)
	{
		if((ft_strcmp1(word_pref(curr->cont_name,word_b),word_b)== 0)&&(ft_strcmp1(word_suff(curr->cont_name,word_e),word_e)== 0))
		{
			create_conts_node(curr -> cont_name,&goal_list);
		}
		curr = curr->next;
	}
	return(goal_list);
}
t_dconts *expand_wildcard(char *str, t_dconts *conts_list)
{
    int i;
	char *start;
	char *word_b;
	char *word_e;

    i = 0;
	word_b = NULL;
	word_e = NULL;
	start = &str[i];
    while (str[i] != '\0')
    {
		if(str[i] == '*')
		{
			if(i != 0)
				word_b = strdup_range(start,&str[i -1]);
			i++;
			while(str[i] == '*')
				i++;
			start = &str[i];
			while (ft_strchr("\t \"\0",str[i])==NULL)
				i++;
			if (start != &str[i])
				word_e = strdup_range(start, &str[i-1]);
			return(goal_list(conts_list,word_b,word_e));
		}
		i++;
    }
	return(NULL);
}

