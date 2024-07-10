/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:01:19 by codespace         #+#    #+#             */
/*   Updated: 2024/07/10 05:03:56 by codespace        ###   ########.fr       */
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
	{
		err_printf("minishell: malloc error: directory_contents\n");
        return ;
	}
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
    list = ft_calloc(1, sizeof(t_dconts));
    if(!list)
     return (err_printf("minishell: malloc error: directory_contents\n", 2), NULL );
    content = readdir(dd);
    while (content)
    {
        create_conts_node(content->d_name, &list);
        content = readdir(dd);
    }
    return(list);
}

// char *word_suff(char *str, char *word_e)
// {
// 	char *suff;
// 	int i;
// 	if(!str|| !word_e)
// 		return(NULL);
// 	suff= malloc(sizeof(char) *(ft_strlen(word_e)+1));
// 	int l1;
// 	int l2;
// 	if(!str|| !word_e)
// 		return(NULL);
// 	l1 = ft_strlen(str)-1;
// 	l2 = ft_strlen(word_e)-1;
// 	i =0;
// 	while(l1 && l2 >= 0)
// 	{
// 		suff[i]= str[l1];
// 		l1--;
// 		l2--;
// 		i++;
// 	}
// 	suff[i] = '\0';
// 	return(ft_strrev(suff));
// }
// char *word_pref(char *str, char *word_b)
// {
// 	int i;
// 	char *pref;

// 	i =0;
// 	if(!str || !word_b)
// 		return(NULL);
// 	pref = malloc(sizeof(char)*(ft_strlen(word_b)+ 1));
// 	while( word_b[i]!= '\0' && str[i] != '\0')
// 	{
// 		pref[i] =str[i];
// 		i++;
// 	}
// 	pref[i] = '\0';
// 	return(pref);
// }
// t_dconts *goal_list(t_dconts *conts_list,char *word_b,char *word_e)
// {
// 	t_dconts *curr;
// 	t_dconts *goal_list;

// 	curr = conts_list;
// 	goal_list = ft_calloc(1,sizeof(t_dconts));
// 	while(curr)
// 	{
// 		if ((ft_strcmp1(word_pref(curr->cont_name,word_b),word_b)== 0)&&(ft_strcmp1(word_suff(curr->cont_name,word_e),word_e)== 0))
// 		{
// 			create_conts_node(curr -> cont_name,&goal_list);
// 		}
// 		curr = curr->next;
// 	}
// 	return(goal_list);
// }
// t_dconts *expand_wildcard(char *str, t_dconts *conts_list)
// {
//     int i;
// 	char *start;
// 	char *word_b;
// 	char *word_e;

//     i = 0;
// 	word_b = NULL;
// 	word_e = NULL;
// 	start = &str[i];
//     while (str[i] != '\0')
//     {
// 		if(str[i] == '*')
// 		{
// 			if(i != 0)
// 				word_b = strdup_range(start,&str[i -1]);
// 			i++;
// 			while(str[i] == '*')
// 				i++;
// 			start = &str[i];
// 			while (ft_strchr("\t \"\0",str[i])==NULL)
// 				i++;
// 			if (start != &str[i])
// 				word_e = strdup_range(start, &str[i-1]);
// 			return(goal_list(conts_list,word_b,word_e));
// 		}
// 		i++;
//     }
// 	return(NULL);
// }

/*	match_pattern_str checks whether str matches pattern
	- calls itself recursively to check 'remainder' of pattern with 'remainder' of str 
	- returns 0 if match; 1 otherwise */
int	match_pattern_str(char *pattern, char *str)
{
	while (*pattern)
	{
		if (*pattern == *str)
		{
			pattern++;
			str++;
		}
		else if (*pattern == '*')
		{
			pattern++;
			if (*pattern == '\0')
				return (0);
			while (*str)
			{
				if (match_pattern_str(pattern, str) == 0)
					return (0);
				str++;
			}
			return (1);
		}
		else
			return (1);
	}
	if (*str == '\0')
		return (0);
	else
		return (1);
}

/*	match_pattern_list c*/
int	match_pattern_list(char *pattern, t_dconts *list)
{
	int	i;

	i = 0;
	while (pattern[i])
	{
		
		i++;
	}
}

char	*expand_wildcard(char *pattern, t_dconts *list)
{
	pattern = remove_quote_str(pattern);
	if (!pattern)
		return (NULL); // terminate shell
	if (match_wc_pattern(pattern, list) == 0)
	{

	}
	else
	{
		
	}
	
}

void	free_conts_list(t_dconts *list)
{
	t_dconts	*current;
	t_dconts	*next;

	current = list;
	while (current)
	{
		next = current->next;
		free(current->cont_name);
		free(current);
		current = next;
	}
}

void	print_conts_list(t_dconts *list)
{
	t_dconts	*current;

	current = list;
	while (current)
	{
		ft_printf("file: %s\n", current->cont_name);
		current = current->next;
	}
}

/*	contain_wc checks whether str contains a wildcard expansion 
	i.e., unquoted asterisk (*) */
int contain_wc(char *str)
{
	int		i;
	char	quote;
	
	i = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == 39 || str[i] == '"')
		{
			quote = str[i];
			i++;
			while (str[i] != '\0' && str[i] != quote)
				i++;
		}
		else if (str[i] == 42)
			return (0);
		i++;
	}
	return (1);
}

/*	file_list_check_wc calls contain_wc and expand_wildcard
	for each file provided in file linked list
	*/
int	file_list_check_wc(t_file *file, t_shell *shell)
{
	t_file	*curr_file;

	curr_file = file;
	while (curr_file)
	{
		if (contain_wc(curr_file->file_name) == 0)
		{
			printf("wc\n");
			(void)shell;
			// curr_file->file_name = expand_wildcard(curr_file->file_name, shell);
			if (!curr_file->file_name)
				return (1);
		}
		curr_file = curr_file->next;
	}
	return (0);
}

/*	check_wc_expansion checks whether wildcard expansions are needed
	- loads and eventually frees shell->directory_contents
	- checks strings in cmd, args and files
	- if needed, expands them
	- returns 1 if errors with expansion
	*/
// work in progress: to check use of create_conts_list and free_conts_list
// work in progress: change this to adjust for commands overflowing to arguments
int	check_wc_expansion(t_ast *node, t_shell *shell)
{
	t_list	*curr_arg;

	shell->directory_contents= create_conts_list();
	if (contain_wc(node->cmd) == 0)
	{
		printf("wc\n");
		// node->cmd = expand_wildcard(node->cmd, shell);
		if (!node->cmd)
			return (free_conts_list(shell->directory_contents), 1);
	}
	if (node->n_args > 0)
	{
		curr_arg = node->args;
		while (curr_arg)
		{
			if (contain_wc(curr_arg->content) == 0)
			{
				printf("wc\n");
				// curr_arg->content = expand_wildcard(curr_arg->content, shell);
				if (!curr_arg->content)
					return (free_conts_list(shell->directory_contents), 1);
			}
			curr_arg = curr_arg->next;
		}
	}
	if (node->input_list)
	{
		if (file_list_check_wc(node->input_list, shell) == 1)
			return (free_conts_list(shell->directory_contents), 1);
	}
	if (node->heredoc_list)
	{
		if (file_list_check_wc(node->heredoc_list, shell) == 1)
			return (free_conts_list(shell->directory_contents), 1);
	}
	if (node->output_list)
	{
		if (file_list_check_wc(node->output_list, shell) == 1)
			return (free_conts_list(shell->directory_contents), 1);
	}
	return (0);
}
