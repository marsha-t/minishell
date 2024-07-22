/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:55:30 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/22 15:53:39 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	event(void)
{
	return (0);
}

// this working perfectly to main process and execve
void handle_cc_signal(int sig)
{
	(void)sig;
	if(LOC == 2)
	{
		LOC = 1;
	}
	if (LOC == 1)
	{
		write(1,"^C\n",3);
		rl_replace_line("",0);
		rl_on_new_line();
		rl_done = 1;
	}
}
void handle_cs_signal(int sig)
{
	(void)sig;
	if (LOC == 0)
	{
		LOC = 1;
		write(1,"Quit\n",5);
		// rl_replace_line("",0);
		// rl_on_new_line();
		// rl_done = 1;

	}
}

void  control_signals(void)
{
	rl_catch_signals = 0;
	rl_event_hook = event;
	signal(SIGINT,handle_cc_signal);
	signal(SIGQUIT,handle_cs_signal);
}

