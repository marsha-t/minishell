/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_files.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 10:11:04 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/19 16:37:05 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void  open_files (t_ast *node)
{
	t_file *current;
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	current = node ->output_list;
	while (current)
	{
		if (access(current ->file_name, F_OK) != 0)
			current->fd =	open(current ->file_name,O_CREAT,mode);
		else if (current ->flag == TOKEN_OUTPUT && access(current ->file_name, F_OK) == 0)
			current->fd =	open(current ->file_name,O_TRUNC,mode);
		current = current -> next;
	}
}

int get_outfile(t_ast *node)
{
	t_file *current;
	int target_fd;

	current = node ->output_list;
	open_files (node);
	if(!current)
		return(1);
	while (current && current -> next)
		current = current ->next;
	if (current ->flag == TOKEN_OUTPUT)
		current->fd =	open(current ->file_name,O_RDWR);
	else if (current ->flag == TOKEN_APPEND)
		current->fd =	open(current ->file_name,O_APPEND|O_RDWR);
	node ->tmp_stdout_fd = dup(STDOUT_FILENO);
	target_fd = dup2(current -> fd , STDOUT_FILENO);
	if(target_fd == -1)
		return(1);
	return(0);
}
void close_files(t_ast *node)
{
	t_file *current;
	current = node->output_list;
	while (current)
	{
		if (current ->fd != -1)
			close(current ->fd);
		current = current ->next;
	}
}
