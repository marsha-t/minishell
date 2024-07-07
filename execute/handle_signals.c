/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:55:30 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/07 21:19:47 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
// this working perfectly to main process and execve
void handle_cc_signal(int sig)
{
	(void)sig;
	rl_catch_signals = 0;
	if (LOC == 1)
	{
		write(1,"^C\n",3);
		rl_on_new_line();
		rl_replace_line("",0);
		rl_redisplay();
	}
	if(LOC == 2)
	{

		write(1,"^C\n",3);
		rl_on_new_line();
		rl_redisplay();
		printf("im here this is loc%d\n",LOC);
	}
}
// this is working good with cat and wc but not with heredoc
// void handle_cc_signal(int sig)
// {
// 	(void)sig;
// 	write(1,"\n",1);
// 	rl_replace_line("",0);
// 	 rl_redisplay();
// }
void handle_cs_signal(int sig)
{
	(void)sig;
	if (LOC == 0)
	{
		write(1,"Quit\n",5);
		rl_replace_line("",0);
		rl_on_new_line();
	 	rl_redisplay();
	}
}

void  control_signals(void)
{
	rl_catch_signals = 0;
	signal(SIGINT,handle_cc_signal);
	signal(SIGQUIT,handle_cs_signal);
	if (LOC == 2)
		LOC = 123;
}

