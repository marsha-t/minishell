/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redir_lists.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 14:56:37 by codespace         #+#    #+#             */
/*   Updated: 2024/06/01 19:22:48 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int create_in_list(t_token *token, t_ast **node)
{
    t_file *current;
    t_file *new;

    new = malloc(sizeof(t_file));
    if(!new)
        return(1);
    new -> flag = 4;
    new->file_name= ft_strdup(token->str); 
    new ->next = NULL;
     current = (*node)-> input_list;
    if( current == 0)
    { 
        (*node)->input_list = new;
        return(0);
    }
    while(current -> next)
        current = current ->next;
    current ->next = new;
    return(0);   
}

int create_heredoc_list(t_token *token, t_ast **node)
{
    t_file *current;
    t_file *new;

    new = malloc(sizeof(t_file));
    if(!new)
        return(1);
    new -> flag = 7;
    new->file_name= ft_strdup(token->str); 
    new ->next = NULL;
     current = (*node)-> heredoc_list;
    if( current == 0)
    { 
        (*node)->heredoc_list = new;
        return(0);
    }
    while(current -> next)
        current = current ->next;
    current ->next = new;
    return(0);   
}

int create_output_append_list(int code, t_token *token, t_ast **node)
{
    t_file *current;
    t_file *new;

    new = malloc(sizeof(t_file));
    if(!new)
        return(1);
    new -> flag = code ;
    new->file_name= ft_strdup(token->str); 
    new ->next = NULL;
     current = (*node)-> output_list;
    if( current == 0)
    { 
        (*node)->output_list = new;
        return(0);
    }
    while(current -> next)
        current = current ->next;
    current ->next = new;
    return(0);   
}