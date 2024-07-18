/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:57:08 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/18 06:14:47 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// since execv only deal with the standard input and standard output get_infile try to lkie make the given input file
// temporary standard input and after executing the command we should return everything back
#include "../minishell.h"

// work in progress: sort out what needs to terminate and what doesn't; make sure all system calls are protecte
int	get_infile(t_ast *node)
{
	int 	fd;
	int 	target_fd;
	t_file	*current;

	current = node ->input_list;
	if (!current)
		return (0);
	while (current && current->next)
	{
		if (access(current->file_name, F_OK) != 0)
		{
			err_printf("%s: No such file or directory\n", current->file_name);
			return (1);
		}
		current = current ->next;
	}
	if(access(current->file_name, F_OK) != 0)
	{
		err_printf("%s: No such file or directory\n", current->file_name);
		return (1);
	}
	fd = open(current ->file_name, O_RDONLY);
	if (fd == -1)
		return (1);
	node ->tmp_stdin_fd = dup(STDIN_FILENO);
	if (node ->tmp_stdin_fd == -1)
		return (1);
	target_fd = dup2(fd, STDIN_FILENO);
	close(fd);
	if (target_fd == -1)
		return (1);
	return (0);
}
