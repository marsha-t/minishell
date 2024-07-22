/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:57:08 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/22 14:00:06 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	err_cons(char *file_name, t_shell *shell)
{
	err_printf("%s: No such file or directory\n", file_name);
	return (shell->file_err = 1, 1);
}

int	get_infile(t_ast *node, t_shell *shell)
{
	int		fd;
	t_file	*current;

	current = node ->input_list;
	if (!current)
		return (0);
	while (current && current->next && shell->file_err != 1)
	{
		if (access(current->file_name, F_OK) != 0)
			return (err_cons(current->file_name, shell));
		current = current ->next;
	}
	if (shell->file_err == 1)
		return (1);
	if (access(current->file_name, F_OK) != 0)
		return (err_cons(current->file_name, shell));
	fd = open(current ->file_name, O_RDONLY);
	if (fd == -1)
		return (err_printf("%s\n", strerror(errno)), 1);
	return (fd);
}

int	dup_input(t_shell *shell, t_ast	*node, int fd)
{
	int	target_fd;

	target_fd = 0;
	node->tmp_stdin_fd = dup(STDIN_FILENO);
	if (node->tmp_stdin_fd == -1)
		return (err_syscall(shell, "dup"));
	target_fd = dup2(fd, STDIN_FILENO);
	if (close(fd) == -1)
		return (err_syscall(shell, "close"));
	if (target_fd == -1)
		return (err_syscall(shell, "dup2"));
	return (0);
}
