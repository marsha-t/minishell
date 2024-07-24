/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:55:30 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/24 12:55:07 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	event(void)
{
	return (0);
}

void	handle_cc_signal(int sig)
{
	(void)sig;
	if (g_loc == 2 || g_loc == 0)
	{
		g_loc = 1;
	}
	if (g_loc == 1)
	{
		g_loc = 130;
		write(1, "^C\n", 3);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_done = 1;
	}
}

void	handle_cs_signal(int sig)
{
	(void)sig;
	if (g_loc == 0)
	{
		g_loc = 131;
		write(1, "Quit\n", 5);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_done = 1;
	}
}

void	control_signals(void)
{
	rl_catch_signals = 0;
	rl_event_hook = event;
	signal(SIGINT, handle_cc_signal);
	signal(SIGQUIT, handle_cs_signal);
}
