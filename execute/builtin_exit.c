/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 10:54:54 by mateo             #+#    #+#             */
/*   Updated: 2024/06/28 00:00:38 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	ft_atoi_ull converts str to unsigned long long */
unsigned long long	ft_atoi_ull(char *str)
{
	unsigned long long	num;

	num = 0;
	if (*str == '-' || *str == '+')
		str++;
	while (ft_isdigit(*str))
	{
		num = num * 10 + (*str - '0') ;
		str++;
	}
	return (num);
}
/*	check_ll_limit converts str to unsigned long long (ft_atoi_ull)
	and returns 1 if number is outside long long limits 
	*/
int check_ll_limit(char *str, int sign)
{
	unsigned long long num;

	num = ft_atoi_ull(str);
	if (sign > 0 && num > LLONG_MAX)
		return (1);
	if (sign < 0 && num > (unsigned long long)LLONG_MAX + 1)
		return (1);
	return (0);
}

/*	check_exit_arg checks whether a numeric argument was provided to exit
	- returns 1 if empty str, non-numeric characters given (other than starting + or -), 
		or number is outside of long long limits*/
int	check_exit_arg(char *str)
{
	int	i;
	int	sign;
	
	i = 0;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (str[i] == '\0')
		return (1);
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (1);
		i++;
	}
	return (check_ll_limit(str, sign));
}

/*	get_exit_status converts str to long long 
	and applies modulo of 256 to get exit status
	- for negative values, need to deduct from 256 to get the same values as bash does */
int	get_exit_status(char *str)
{
	long long	num;
	int	sign;

	sign = 1;
	if (*str == '-')
		sign = -1;
	num = (long long) ft_atoi_ull(str) * sign;
	num = num % 256;
	if (num < 0)
		num = 256 - num;
	return ((int) num);
}

/*	exit_shell does final clean-up before shell is exited
	- free shell
	- clear history */
// work in progress: error running rl_clear_history on Mac (error: implicit declaration)
void	exit_shell(t_shell *shell)
{
	free_shell(shell);
	// rl_clear_history();
	exit(shell->exit_status);
}

/*	builtin_exit runs the exit command
	- checks validity of arguments
	- determines value (exit status) to return
	- frees everything allocated so far
	- clears history
	- errors: 
		- numeric argument required: exit status = 2
		- too many arguments: exit status = 1 */
int	builtin_exit(t_ast *node, t_shell *shell)
{
	int	exit_status;

	if (node->n_args > 0 && check_exit_arg(node->args->content) == 1)
		return (ft_putstr_fd("exit: numeric argument required\n", 2), 2);
	if (node->n_args > 1)
		return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
	if (node->n_args == 0)
		exit_status = 0;
	else
		exit_status = get_exit_status(node->args->content);
	exit_shell(shell);
	return (exit_status);
}
