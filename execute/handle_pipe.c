/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 13:44:43 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/28 21:48:47 by ryagoub          ###   ########.fr       */
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


int init_pipe( t_shell *shell)
{
	int i;
	i = 0;
	printf("nums of pipes %d \n",nums_of_pipes(shell->line));
	shell -> pipe_data = malloc(sizeof(t_pipe_info));
	shell ->pipe_data->pipe_count = nums_of_pipes(shell->line);
	shell->pipe_data->pid = malloc(sizeof(int *)*shell->pipe_data->pipe_count + 1);
	if (!shell ->pipe_data->pid)
		return(1);
	shell->pipe_data->pipes = malloc(sizeof(int *)*shell->pipe_data->pipe_count);
	if (!shell ->pipe_data->pipes)
		return(1);
	while( i < shell->pipe_data-> pipe_count)
	{
		shell->pipe_data->pipes[i] = malloc(sizeof(int)*2);
		shell ->pipe_data-> pipes[i][0] = -2;
		shell -> pipe_data->pipes[i][1] = -2;
		i++;
	}
	return(0);
}
int piping_first_one(pid_t *pid, int *k, t_ast *node, t_shell *shell)
{
	pid[*k] = fork();
	if (pid[*k] == 0)
	{
		close (shell-> pipe_data->pipes[shell->pipe_data -> pipe_count - 1][0]);
		dprintf(2, "%d\n",shell->pipe_data -> pipe_count );

		if(dup2(shell->pipe_data->pipes[shell->pipe_data -> pipe_count - 1 ][1], STDOUT_FILENO) == -1)
			return(1);
		close (shell-> pipe_data->pipes[shell->pipe_data -> pipe_count - 1][1]);
		if (shell->pipe_data -> pipe_count != 1)
		{
			close (shell-> pipe_data->pipes[shell->pipe_data -> pipe_count - 2][0]);
			close (shell-> pipe_data->pipes[shell->pipe_data -> pipe_count - 2][1]);
		}
		execute_cmd_node(node -> left,shell);
		return (0);
	}
	else
	{
		dprintf(2, "%d\n",shell->pipe_data -> pipe_count );
		(*k)++;
		pid[*k] = fork();
		if(pid[*k] == 0)
		{
			dprintf(2, "im here in the right process\n");
			if(dup2(shell->pipe_data -> pipes[shell->pipe_data -> pipe_count - 1][0],STDIN_FILENO)== -1)
				return(dprintf(2, "im here in the right process\n"),1);
			if (shell->pipe_data -> pipe_count != 1)
			{
				if(dup2(shell->pipe_data->pipes[shell->pipe_data -> pipe_count - 1][1], STDOUT_FILENO) == -1)
					return(1);
				// dprintf(2, "fd closed is %d\n",shell-> pipe_data->pipes[shell->pipe_data -> pipe_count - 1][0]);
				// dprintf(2, "fd closed is %d\n",shell-> pipe_data->pipes[shell->pipe_data -> pipe_count - 1][1]);
				// dprintf(2, "fd closed is %d\n",shell-> pipe_data->pipes[shell->pipe_data -> pipe_count - 2][0]);
				// dprintf(2, "fd closed is %d\n",shell-> pipe_data->pipes[shell->pipe_data -> pipe_count - 2][1]);
				close (shell-> pipe_data->pipes[shell->pipe_data -> pipe_count - 1][0]);
				close (shell-> pipe_data->pipes[shell->pipe_data -> pipe_count - 1][1]);
				close (shell-> pipe_data->pipes[shell->pipe_data -> pipe_count -2][0]);
				close (shell-> pipe_data->pipes[shell->pipe_data -> pipe_count - 2][1]);
			}
			else
			{
				close (shell-> pipe_data->pipes[shell->pipe_data -> pipe_count - 1][0]);
				close (shell-> pipe_data->pipes[shell->pipe_data -> pipe_count - 1][1]);
			}
			execute_cmd_node(node -> right, shell);
			return(0);
		}

	}
	// else
	// {
	// 	close (node -> pipes[node -> pipe_count - 1][0]);
	// 	close (node -> pipes[node -> pipe_count - 1][1]);
	// }
	return(0);
}
int piping(t_ast *node, t_shell *shell, int i, pid_t pid)
{
	dprintf(2, "im here in piping\n");
	dprintf(2, " this is i  in piping%d \n ", i);
	dprintf(2,"this is the first command in piping %s\n",node ->right -> cmd);
	dprintf(2, "%d\n",shell->pipe_data -> pipe_count );
	pid = fork();
	if(pid == 0)
	{
		close (shell->pipe_data -> pipes[i][1]);
		dup2(shell->pipe_data -> pipes[i][0], STDIN_FILENO);
		close(shell->pipe_data -> pipes[i][0]);
		if(i != 1)
		{
			dprintf(2, "in if this is i  in piping child%d \n ", i);
			dup2(shell->pipe_data-> pipes[i - 1][1], STDOUT_FILENO);
		}
		close(shell->pipe_data -> pipes[i - 1][1]);
		dprintf(2, " this is i  in piping child%d \n ", i);
		close(shell->pipe_data -> pipes[i - 1][0]);

		// dprintf(2, "fd closed is %d\n",shell-> pipe_data->pipes[i][0]);
		// dprintf(2, "fd closed is %d\n",shell-> pipe_data->pipes[i][1]);
		// dprintf(2, "fd closed is %d\n",shell-> pipe_data->pipes[i - 1][0]);
		// dprintf(2, "fd closed is %d\n",shell-> pipe_data->pipes[i - 1][1]);

		execute_cmd_node(node -> right, shell);
	}
	return(0);
}
int handle_pipe(t_ast *node , t_shell *shell)
{
	static int i;
	static int k;

	if(node ->code != 3)
		return(0);
	if ( i < shell -> pipe_data->pipe_count)
	{
		pipe(shell->pipe_data->pipes[i]);}
	i++;
	if(node -> left && node -> left -> code == TOKEN_PIPE)
	{
		handle_pipe(node -> left, shell);
		i--;
	}
	if(i == shell -> pipe_data-> pipe_count)
		piping_first_one(shell->pipe_data ->pid, &k, node,shell);
	else
		piping(node,shell,i,shell ->pipe_data ->pid[k]);
	k++;
	dprintf(2,"the value of k is %d and I am call %d\n", k, i);
	// if(k ==shell -> pipe_data-> pipe_count + 1)
	if(i == 1)
	{
		dprintf(2, "this should be printed only once\n");
		int j;
		j = 0;
		while (j < shell ->pipe_data->pipe_count)
		{
			close (shell ->pipe_data-> pipes[j][0]);
			close (shell ->pipe_data-> pipes[j][1]);
			// dprintf(2, "main closed %d\n", shell ->pipe_data-> pipes[j][0]);
			// dprintf(2, "main closed %d\n", shell ->pipe_data-> pipes[j][1]);
			j++;
		}
		j = 0;
		while (j <= shell ->pipe_data->pipe_count)
		{
			dprintf(2, "im here in the waiting%d\n",j);
			// waitpid(shell->pipe_data->pid[j], &shell ->exit_status, 0);
			wait(NULL);
			j++;
		}
		k = 0;
		i = 0;
	}
	// int j = 0;
	// while (j < shell ->pipe_data->pipe_count)
	// {
	// 	close (shell ->pipe_data-> pipes[j][0]);
	// 	close (shell ->pipe_data-> pipes[j][1]);
	// 	dprintf(2, "main closed %d\n", shell ->pipe_data-> pipes[j][0]);
	// 	dprintf(2, "main closed %d\n", shell ->pipe_data-> pipes[j][1]);
	// 	j++;
	// }
	return(0);
}




// int make_pipe(t_ast *node, int pipe_count, int i, t_shell *shell,int  write_fd)
// {
// 	if(i == pipe_count)
// 	{
// 		dup2(write_fd, STDOUT_FILENO);
// 		close(write_fd);
// 		execute_cmd_node(node -> left ,shell);
// 	}
// 	dup2(node ->read_fd , STDIN_FILENO);
// 	dup2(write_fd, STDOUT_FILENO);
// 	dprintf(2, "\n%d this is the writing fd\n", write_fd);
// 	execute_cmd_node(node -> left , shell);
// 	return(0);
// }

// int handle_pipe(t_ast *node, t_shell *shell)
// {

// 	int pipe_count;
// 	static int i;
// 	static int k;
// 	int fd[2];
// 	if(node ->code != 3)
// 		return(0);
// 	pipe_count = nums_of_pipes(shell -> line);
// 	int pid[pipe_count + 1];
// 	dprintf(2, "this is how much I malloc %d", pipe_count + 1);
// 	i++;
// 	if(node -> left -> code == TOKEN_PIPE)
// 	{
// 		node->read_fd = handle_pipe(node -> left, shell);
// 	}
// 	// else
// 	// {
// 		pipe(fd);
// 		node -> write_fd = fd[1];
// 		// node -> read_fd = fd[0];
// 		if (i == pipe_count)
// 		{
// 			pid[k] = fork();
// 			k++;
// 			if( pid[k - 1] == 0)
// 				make_pipe(node, pipe_count, i, shell, fd[1]);
// 		}
// 		// else
// 		// {
// 		// node -> write_fd = fd[1];
// 		// node -> read_fd = fd[0];
// 		pid[k] = fork();
// 		k++;
// 		if (pid[k - 1] == 0)
// 		{
// 			close (fd[1]);
// 			dup2(node -> read_fd, STDIN_FILENO);
// 			execute_cmd_node(node -> right , shell);
// 		}
// 		else
// 		{
// 			if (k == pipe_count + 1)
// 			{
// 				close(fd[0]);
// 				close(fd[1]);
// 				int j;
// 				j = 0;
// 				printf("THERE IS A k and this is its value %d\n", k);
// 				while (j < k)
// 				{
// 					printf("I am about to wait for this guy pid[%d] = %d\n", j, pid[j]);
// 					waitpid(pid[j],&shell->exit_status, 0);
// 					j++;
// 				}
// 				printf("yaaaay i finished waiting\n");
// 				k = 0;
// 				i = 0;
// 				return(0);
// 			}
// 			else
// 			{
// 				printf("im here in this return\n");
// 				node -> read_fd = fd[0];
// 				return(node->read_fd);
// 			}
// 		// }
// 		}
// 	// }
// 	return(0);
// }
























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
