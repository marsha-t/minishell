/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_files.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 10:11:04 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/22 10:49:18 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_files(t_ast *node, t_shell *shell)
{
	t_file *current;
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	struct stat	f_stat;

	current = node->output_list;
	while (current->next && shell->file_err != 1)
	{
		if (access(current->file_name, F_OK) != 0)
		{
			current->fd = open(current ->file_name, O_CREAT, mode);
			if (current->fd == -1)
				return (shell->file_err = 1,err_printf("%s\n", strerror(errno)), 1);
		}
		else if (current->flag == TOKEN_OUTPUT && access(current ->file_name, F_OK) == 0)
		{
			if (stat(current->file_name, &f_stat) == -1)
				return (err_syscall(shell, "stat"));
			if (S_ISDIR(f_stat.st_mode))
				return (shell->file_err =1,err_printf("%s: this is a directory\n", current->file_name),1);
			current->fd = open(current->file_name, O_TRUNC,mode);
			if (current->fd == -1)
				return (shell->file_err =1,err_printf("%s\n", strerror(errno)), 1);
		}
		if(shell->file_err == 1)
				return(1);		// if (current->fd == -1)
		// 	return (err_syscall(shell, "open"));
		if (close(current->fd) == -1)
			return (err_syscall(shell, "close"));
		current = current -> next;
	}
	return (0);
}

int	get_outfile(t_ast *node, t_shell *shell)
{
	t_file		*current;
	// int			target_fd;
	mode_t		mode;
	struct stat	f_stat;

	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	current = node ->output_list;
	if (!current)
		return (0);
	if (open_files(node, shell) == 1)
		return (1);
	while (current && current->next)
		current = current->next;
	if(shell->file_err == 1)
		return(1);
	stat(current->file_name, &f_stat);
	if (S_ISDIR(f_stat.st_mode))
		return (shell->file_err =1,err_printf("%s: Is a directory\n", current->file_name), 1);
	if (current->flag == TOKEN_OUTPUT)
		current->fd = open(current ->file_name, O_CREAT | O_TRUNC | O_RDWR, mode);
	else if (current ->flag == TOKEN_APPEND)
	{
		current->fd = open(current ->file_name, O_CREAT | O_APPEND | O_RDWR, mode);
	}
	if (current->fd == -1)
		{return (shell->file_err =1,err_printf("%s\n", strerror(errno)), 1);}
	return(current->fd);
}

int dup_output(t_shell *shell, t_ast *node, int o_fd)
{
	int target_fd ;
	target_fd =0;
	node->tmp_stdout_fd = dup(STDOUT_FILENO);
	if (node->tmp_stdout_fd == -1)
		return (err_syscall(shell, "dup2"));
	target_fd = dup2(o_fd, STDOUT_FILENO);
	if (close(o_fd) == -1)
		return (err_syscall(shell, "close"));
	if (target_fd == -1)
		return (err_syscall(shell, "dup2"));
	return (0);
}

// void close_files(t_ast *node)
// {
// 	t_file *current;
// 	current = node->output_list;
// 	while (current)
// 	{
// 		if (current ->fd != -1)
// 			close(current ->fd);
// 		current = current ->next;
// 	}
// }
