/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 13:44:43 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/23 04:52:12 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int nums_of_pipes(char * line)
{
	int i;
	int pipes_count;

	pipes_count = 0;
	i = 0;
	if(!line)
		return(0);
	while(line[i]!= '\0')
	{
		if(line[i] == '|')
			pipes_count++;
		i++;
	}
	return(pipes_count);
}
int make_pipe(t_ast *node, int i, t_shell *shell, int pipes_count)
{
	int fd[2];
	pipe(fd);
	node ->read_fd = fd[0];
	node -> write_fd = fd[1];
	if(i == 0)
	{
		printf("im here\n");
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execute_cmd_node(node -> left ,shell);
	}
	if (i == pipes_count)
	{
		dup2(fd[1], STDOUT_FILENO);
		execute_cmd_node(node -> left,shell);
	}
	dup2(node ->read_fd , STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	execute_cmd_node(node -> left , shell);
	return(0);
}

int handle_pipe(t_ast *node, t_shell *shell)
{
    int pid;
	int pipe_count;
	static int i;
	int fd[2];
	if(node ->code != 3)
		return(0);
	pipe_count = nums_of_pipes(shell -> line);
	if(node -> left -> code == TOKEN_PIPE)
		{
			i++;
			handle_pipe(node -> left, shell);}
	else
	{
		pid = fork();
		if( pid == 0)
			make_pipe(node, i, shell,pipe_count);
		else
		{
			pid = fork();
			if (pid == 0)
			{
				pipe(fd);
				dup2(fd[1], STDOUT_FILENO);
				execute_cmd_node(node -> left , shell);
			}
			else
			{
				int j;
				j = 0;
				while (J < nums_of_pipes)
			}
			
		}
	}
	return(0);
}
























//     if (pipe(fd) == -1)
//       return(ft_printf("error in creating the pipe %s \n", strerror(errno)),1);
//     pid[0] = fork();
// 	if(pid[0] == 0 )
// 	{
// 		close(fd[0]);
// 		dup2(fd[1], STDOUT_FILENO);
// 		execute_cmd_node(node -> left, shell);
// 		exit (1);
// 		// close (fd[1]);
// 	}
// 	else
// 	{
// 		pid [1] = fork();
// 		if (pid[1] == 0)
// 		{
// 			close(fd[1]);
// 			dup2(fd[0], STDIN_FILENO);
// 			execute_cmd_node(node-> right,shell);
// 			exit(1);
// 		}
// 		close(fd[0]);
// 		close(fd[1]);
// 		waitpid(pid[0],&status,0);
// 		waitpid(pid[1],&status,0);
// 	}
// 	return(0);
// }
