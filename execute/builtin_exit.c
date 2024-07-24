/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 10:54:54 by mateo             #+#    #+#             */
/*   Updated: 2024/07/22 21:46:11 by ryagoub          ###   ########.fr       */
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
		num = num * 10 + (*str - '0');
		str++;
	}
	return (num);
}

/*	check_ll_limit converts str to unsigned long long (ft_atoi_ull)
	and returns 1 if number is outside long long limits
	*/
int	check_ll_limit(char *str, int sign)
{
	unsigned long long	num;

	num = ft_atoi_ull(str);
	if (sign > 0 && num > LLONG_MAX)
		return (1);
	if (sign < 0 && num > (unsigned long long)LLONG_MAX + 1)
		return (1);
	return (0);
}

/*	check_exit_arg checks whether a numeric argument was provided to exit
	- returns 1 if empty str, non-numeric characters given
		(other than starting + or -),
		or number is outside of long long limits */
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
		return (err_printf("exit: %s: numeric argument required\n",
				node->args->content), 2);
	if (node->n_args > 1)
		return (err_printf("exit: too many arguments\n"), 1);
	if (node->n_args == 0)
		exit_status = 0;
	else
		exit_status = get_exit_status(node->args->content);
	exit_shell(shell, exit_status);
	return (exit_status);
}
