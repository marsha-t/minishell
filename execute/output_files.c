/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_files.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 10:11:04 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/24 15:32:18 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_and_open(t_shell *shell, t_file*current, int *opened)
{
	struct stat	f_stat;

	if (stat(current->file_name, &f_stat) == -1)
		return (err_syscall(shell, "stat"));
	if (S_ISDIR(f_stat.st_mode))
		return (shell->file_err = 1,
			err_printf("%s: Is a directory\n", current->file_name), 1);
	current->fd = open(current->file_name, O_TRUNC, 0777);
	if (current->fd == -1)
		return (shell->file_err = 1,
			err_printf("%s\n", strerror(errno)), 1);
	*opened = 1;
	return (0);
}

int	open_files(t_ast *node, t_shell *shell)
{
	t_file		*current;
	int			opened;

	current = node->output_list;
	while (current->next && shell->file_err != 1)
	{
		opened = 0;
		if (access(current->file_name, F_OK) != 0)
		{
			current->fd = open(current ->file_name, O_CREAT, 0777);
			if (current->fd == -1)
				return (shell->file_err = 1,
					err_printf("%s\n", strerror(errno)), 1);
			opened = 1;
		}
		else if (current->flag == TOKEN_OUTPUT)
			if (check_and_open(shell, current, &opened) == 1)
				return (1);
		if (shell->file_err == 1)
			return (1);
		if (opened == 1 && close(current->fd) == -1)
			return (err_printf("%s \n", strerror(errno)), 1);
		current = current -> next;
	}
	return (0);
}

int	get_outfile(t_ast *node, t_shell *shell)
{
	t_file		*current;
	struct stat	f_stat;

	current = node ->output_list;
	if (!current)
		return (0);
	if (open_files(node, shell) == 1)
		return (1);
	while (current && current->next)
		current = current->next;
	if (shell->file_err == 1)
		return (1);
	if (stat(current->file_name, &f_stat) == 0 && S_ISDIR(f_stat.st_mode))
		return (shell->file_err = 1
			, err_printf("%s: Is a directory\n", current->file_name), 1);
	if (current->flag == TOKEN_OUTPUT)
		current->fd = open(current ->file_name,
				O_CREAT | O_TRUNC | O_RDWR, 0644);
	else if (current ->flag == TOKEN_APPEND)
		current->fd = open(current ->file_name,
				O_CREAT | O_APPEND | O_RDWR, 0644);
	if (current->fd == -1)
		return (shell->file_err = 1, err_printf("%s\n", strerror(errno)), 1);
	return (current->fd);
}

int	dup_output(t_ast *node, int o_fd)
{
	int	target_fd ;

	target_fd = 0;
	node->tmp_stdout_fd = dup(STDOUT_FILENO);
	if (node->tmp_stdout_fd == -1)
		return (err_printf("%s \n", strerror(errno)), 1);
	target_fd = dup2(o_fd, STDOUT_FILENO);
	if (close(o_fd) == -1)
		return (err_printf("%s \n", strerror(errno)), 1);
	if (target_fd == -1)
		return (err_printf("%s \n", strerror(errno)), 1);
	return (0);
}
