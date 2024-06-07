/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:01:19 by codespace         #+#    #+#             */
/*   Updated: 2024/06/07 17:14:05 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void create_conts_node(char *str, t_dconts **list)
{
    t_dconts *curr;
    t_dconts *new;
    
    curr = *list;
    new = malloc(sizeof(t_dconts));
    if(!new)
        return ft_putstr_fd("malloc error :create directory_contents\n", 2) ;
    new->cont_name = ft_strdup(str);
    new -> next = NULL;
    if(!*list)
    { 
        *list = new;
        return ;
    }
    while(curr->next)
        curr = curr -> next;
    curr->next = new;
}

t_dconts *create_conts_list(void)
{
    struct dirent *content;
    DIR *dd;
    t_dconts *list;
    
    dd= opendir(".");
    list = malloc(sizeof(t_dconts));
    if(!list)
     return (ft_putstr_fd("malloc error :create directory_contents\n", 2), NULL );
    content = readdir(dd);
    while (content)
    {
        create_conts_node(content -> d_name,&list); 
        content = readdir(dd); 
    }
    t_dconts *curr;
    curr=list;
    while(curr)
    {
        printf("%s   ",curr ->cont_name);
        curr = curr ->next;
    }
    return(list);
}

// char *expand_wildcard(char *str, t_var *var)
// {
//     int i;
    
//     i = 0;
//     while (str[i] != '\0')
//     {
        
//     }
// }