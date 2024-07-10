/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 13:56:32 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/09 17:04:49 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int open_file(t_file *current, int flag)
{
	char	*line;
	if (flag ==1)
		current -> fd = open(current ->file_name, O_CREAT | O_RDWR, 0777);
	line = readline(">");
	if (LOC == 1)
		return(printf("im here in -12\n"),-1);
	while(ft_strcmp(current->file_name,line)&& LOC != 1)
	{
		if (flag ==1)
		{ft_putstr_fd(line,current -> fd);
		ft_putstr_fd("\n",current -> fd);}
		if (LOC != 1)
			line = readline(">");
	}
	if (LOC == 1)
		return(/*printf("im here in -1\n"),*/-1);
	// if(flag ==  0)
	// {
	// 	close(current -> fd);
	// 	unlink(current ->file_name);
	// }
	return(0);
}

int get_docs(t_ast *node)
{
	t_file *current;
	int target_fd;

	LOC =2;
	current = node -> heredoc_list;
	if(!current)
		return(0);
	// line = readline(">");
	while (current && current -> next)
	{
		if(open_file(current, 0)==-1)
			return(1);
		current = current -> next;
	}
	if(open_file(current, 1)==-1)
	{
		/*dprintf(2, "this is my global variable in the child %d\n", LOC);*/
		close(current ->fd);
		unlink(current ->file_name);
		return(1);
	}
	// node -> tmp_stdin_fd = dup(STDIN_FILENO);
	printf ("%d \n",current ->fd );
	current ->fd=open(current->file_name,O_RDONLY,0777);
	target_fd = dup2(current ->fd , STDIN_FILENO);
	close(current -> fd);
	unlink(current ->file_name);
	if (target_fd == -1)
		return(1);
	return(0);
}
