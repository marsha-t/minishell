/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 15:04:20 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/22 16:36:47 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <errno.h>
# include <stdarg.h>
#include <fcntl.h>
# include <dirent.h>
#include <sys/stat.h>
# include "libft/libft.h"
# include "printf/ft_printf.h"
#include <sys/wait.h>
#include <time.h>
#include <string.h>


int main(int ac, char **av , char **envp)
{
    int fd[2];
    int pid;
	char msg[1024];
	char **str;
	char **str1;
	int fd_org[2];

	fd_org[0] = dup(STDIN_FILENO);
	fd_org[1] = dup(STDOUT_FILENO);
	str = malloc(sizeof(char *) * 3);

	str[0] = strdup("/bin/ls");
	str[1] = strdup("check");
	str[2] = NULL;
	str = malloc(sizeof(char *) * 3);

	str1 = malloc(sizeof(char *) * 3);
	str1[0] = strdup("/usr/bin/grep");
	str1[1] = strdup("*.o");
	str1[2] = NULL;
	pipe(fd);
	// int id = fork();
	// if (id == 0)
	dup2(fd[0] ,STDOUT_FILENO);
	write(1, "hllof", 5);
	// else
	// {
	// 	wait(NULL);
	// 	dup2(fd[0],STDOUT_FILENO);
	// }
	// dup2(fd_org[1], STDOUT_FILENO);
	// write(1, "insde _pipe\n", 12);


	// read(STDIN_FILENO, msg, 1000);

	// printf("(%s)", msg);
    // pid = fork();
	// if (pid == 0)
	// {
	// 	dup2(fd[1], STDOUT_FILENO);
	// 	close(fd[0]);
	// 	close(fd[1]);
	// 	execve(str[0], str, envp);
	// }
	// else
	// {
	// 	close(fd[1]);
	// 	dup2(fd[0], STDIN_FILENO);
	// 	execve(str1[0], str1, envp);
	// }


    // if (pid == 0)
    // {
	// 	pid = fork();
	// 	if(pid == 0 )
	// 	{
	// 		close(fd[0]);
	// 		dup2(STDIN_FILENO, fd[1]);
	// 		execute_cmd_node(node -> left, shell);
	// 		// close (fd[1]);
	// 	}
	// 	else
	// 	{
	// 		close(fd[1]);
	// 		dup2(fd[0],STDOUT_FILENO);
	// 		printf("im hereee\n");
	// 		close (fd[0]);
	// 		execute_cmd_node(node->right, shell);
	// 	}
    // }
	// else
	// {
	// 	close(fd[0]);
	// 	close(fd[1]);
	// 	wait(NULL);
	// }
	return(0);
}
