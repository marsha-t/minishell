/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 13:56:32 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/18 15:54:25 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	returns 1 if error
	0 otherwise*/
int open_hd_file(t_file *current, char *uniq_file, t_shell *shell)
{
	char	*line;

	if (uniq_file)
	{
		current->fd = open(uniq_file, O_CREAT | O_RDWR, 0777);
		if (current->fd == -1)
			return (err_syscall(shell, "open"));
	}
	line = readline(">");
	if (!line)
		return (err_syscall(shell, "readline"));
	if (LOC == 1)
		return (free(line), /*ft_printf("im here in -12\n"),*/ 1);
	while (ft_strcmp(current->file_name, line) && LOC != 1)
	{
		if (uniq_file)
		{
			ft_putstr_fd(line,current -> fd);
			ft_putstr_fd("\n",current -> fd);
		}
		if (LOC != 1)
		{
			free(line);
			line = readline(">");
			if (!line)
				return (err_syscall(shell, "readline"), 1);
		}
	}
	free(line);
	if (LOC == 1)
		return (/*ft_printf("im here in -1\n"),*/1);
	// if(flag ==  0)
	// {
	// 	close(current -> fd);
	// 	unlink(current ->file_name);
	// }
	return (0);
}

int get_docs(t_ast *node, t_shell *shell)
{
	t_file	*current;
	int		target_fd;
	char	*uniq_file;
	
	LOC =2;
	current = node->heredoc_list;
	if (!current) 
		return (0);
	while (current && current->next)
	{
		if (open_hd_file(current, NULL, shell) == 1)
			return (1);
		current = current->next;
	}
	uniq_file = ft_strjoin("X9f4$Tp1", current->file_name);
	if (!uniq_file)
		return (err_syscall(shell, "malloc"));
	if (open_hd_file(current, uniq_file, shell) == 1)
	{
		/*dprintf(2, "this is my global variable in the child %d\n", LOC);*/
		if (close(current->fd) == -1)
			return (err_syscall(shell, "close"));
		if (unlink(uniq_file) == -1)
			return (err_syscall(shell, "unlink"));
		free(uniq_file);
		return(1);
	}
	node->tmp_stdin_fd = dup(STDIN_FILENO);
	if (node->tmp_stdin_fd == -1)
		return (free(uniq_file), err_syscall(shell, "dup"));
	current->fd = open(uniq_file, O_RDONLY, 0777);
	if (current->fd == -1)
		return (free(uniq_file), err_syscall(shell, "open"));
	target_fd = dup2(current->fd , STDIN_FILENO);
	if (close(current->fd) == -1)
		return (free(uniq_file), err_syscall(shell, "close"));
	if (unlink(uniq_file) == -1)
		return (free(uniq_file), err_syscall(shell, "unlink"));
	free(uniq_file);
	if (target_fd == -1)
		return (err_syscall(shell, "dup2"));
	return (0);
}
