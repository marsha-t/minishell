/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redir_lists.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 14:56:37 by codespace         #+#    #+#             */
/*   Updated: 2024/06/04 09:26:52 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*  create_in_list
    - returns 1 for malloc error */
int create_in_list(t_token *token, t_ast **node)
{
    t_file *current;
    t_file *new;

    new = malloc(sizeof(t_file));
    if(!new)
        return(ft_putstr_fd("Malloc error creating t_file for in_list\n", 2), 1);
    new->flag = TOKEN_INPUT;
    new->file_name= ft_strdup(token->str);
    if (!new->file_name)
        return (ft_putstr_fd("Malloc error creating t_file->file_name for in_list\n", 2), 1);
    new ->next = NULL;
    current = (*node)-> input_list;
    if (current == 0)
    { 
        (*node)->input_list = new;
        return (0);
    }
    while(current -> next)
        current = current ->next;
    current ->next = new;
    return (0);   
}

/*  create_heredoc_list 
    - returns 1 if malloc error */
int create_heredoc_list(t_token *token, t_ast **node)
{
    t_file *current;
    t_file *new;

    new = malloc(sizeof(t_file));
    if(!new)
        return(ft_putstr_fd("Malloc error creating t_file for heredoc_list\n", 2), 1);
    new -> flag = TOKEN_HEREDOC;
    new->file_name= ft_strdup(token->str); 
    if (!new->file_name)
        return (ft_putstr_fd("Malloc error creating t_file->file_name for heredoc_list\n", 2), 1);
    new->next = NULL;
    current = (*node)->heredoc_list;
    if (current == 0)
    { 
        (*node)->heredoc_list = new;
        return (0);
    }
    while (current->next)
        current = current->next;
    current->next = new;
    return (0);   
}

/*  create_output_append_list  
    - returns 1 if malloc error*/
int create_output_append_list(int code, t_token *token, t_ast **node)
{
    t_file *current;
    t_file *new;

    new = malloc(sizeof(t_file));
    if(!new)
        return(ft_putstr_fd("Malloc error creating t_file for output_list\n", 2), 1);
    new->flag = code;
    new->file_name= ft_strdup(token->str); 
    if (!new->file_name)
        return (ft_putstr_fd("Malloc error creating t_file->file_name for output_list\n", 2), 1);
    new->next = NULL;
     current = (*node)-> output_list;
    if (current == 0)
    { 
        (*node)->output_list = new;
        return (0);
    }
    while(current->next)
        current = current->next;
    current->next = new;
    return (0);   
}