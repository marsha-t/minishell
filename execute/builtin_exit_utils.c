/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 21:41:04 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/22 21:44:05 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
	/*	get_exit_status converts str to long long
	and applies modulo of 256 to get exit status
	- for negative values, need to deduct from 256 to get same values as bash */
int	get_exit_status(char *str)
{
	long long	num;
	int			sign;

	sign = 1;
	if (*str == '-')
		sign = -1;
	num = (long long) ft_atoi_ull(str) * sign;
	num = num % 256;
	if (num < 0)
		num = 256 - (-1 * num);
	return ((int) num);
}

/*	exit_shell does final clean-up before shell is exited
	- free shell
	- clear history */
// work in progress: error running rl_clear_history
// on Mac (error: implicit declaration)
void	exit_shell(t_shell *shell, int exit_status)
{
	free_shell(shell);
	exit(exit_status);
}
