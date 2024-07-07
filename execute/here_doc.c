/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 13:56:32 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/07 21:21:49 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int open_file(t_file *current, int flag)
{
	char	*line;
	LOC =2;
	current -> fd = open(current ->file_name, O_CREAT | O_RDWR, 0777);
	printf("%d\n",current ->fd);
	line = readline(">");
	while(ft_strcmp(current->file_name,line))
	{
		if (flag == 1)
		{
			if(LOC == 123)
			{
				close(current -> fd);
				unlink(current ->file_name);
				exit (EXIT_SUCCESS);
			}
			printf("im here this is loc%d\n",LOC);
		}
		ft_putstr_fd(line,current -> fd);
		ft_putstr_fd("\n",current -> fd);
		line = readline(">");
	}
	if(flag ==  0)
	{
		close(current -> fd);
		unlink(current ->file_name);
	}
	return(0);
}

int get_docs(t_ast *node)
{
	t_file *current;
	int target_fd;

	current = node -> heredoc_list;
	if(!current)
		return(0);
	// line = readline(">");
	while (current && current -> next)
	{
		open_file(current, 0);
		current = current -> next;
	}
	open_file(current, 1);
	// node -> tmp_stdin_fd = dup(STDIN_FILENO);
	close(current ->fd);
	printf ("%d \n",current ->fd );
	current ->fd=open(current->file_name,O_RDONLY,0777);
	target_fd = dup2(current ->fd , STDIN_FILENO);
	close(current -> fd);
	// unlink(current ->file_name);
	if (target_fd == -1)
		return(1);
	return(0);
}
