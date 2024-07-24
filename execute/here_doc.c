/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 13:56:32 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/24 15:40:13 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	returns 1 if error
	0 otherwise*/

int	open_file(t_file *current, int flag, t_shell *shell)
{
	char	*line;

	if (flag == 1)
		current -> fd = open("X9f4Tp1", O_CREAT | O_RDWR, 0777);
	line = readline(">");
	if (!line || g_loc == 130)
		return (free(line), g_loc = 1, shell->file_err = 1, 1);
	while (ft_strcmp(current->file_name, line) && g_loc != 130)
	{
		if (flag == 1)
		{
			if (contain_var(line) == 0)
				expand_str(line, shell);
			ft_putstr_fd(line, current -> fd);
			ft_putstr_fd("\n", current -> fd);
		}
		if (g_loc != 1)
		{
			free(line);
			line = readline(">");
		}
		if (!line)
			return (shell->file_err = 1, 1);
	}
	free(line);
	if (g_loc == 130)
		return (g_loc = 1, shell->file_err = 1, shell->exit_status = 130, 1);
	return (0);
}

int	get_docs(t_ast *node, t_shell *shell)
{
	t_file	*current;

	g_loc = 2;
	current = node -> heredoc_list;
	if (!current)
		return (0);
	while (current && current -> next)
	{
		if (open_file(current, 0, shell) == 1)
			return (1);
		current = current -> next;
	}
	if (open_file(current, 1, shell) == 1)
		return (close(current ->fd), unlink("X9f4Tp1"), 1);
	close(current->fd);
	current ->fd = open ("X9f4Tp1", O_RDONLY, 0777);
	return (current -> fd);
}

int	dup_heredoc(t_ast *node, t_shell *shell, int fd)
{
	int	target_fd;

	node -> tmp_stdin_fd = dup(STDIN_FILENO);
	target_fd = dup2(fd, STDIN_FILENO);
	close(fd);
	unlink("X9f4Tp1");
	if (target_fd == -1)
		return (shell->file_err = 1, 1);
	return (0);
}
