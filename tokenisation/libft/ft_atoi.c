/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 23:33:48 by mateo             #+#    #+#             */
/*   Updated: 2024/01/03 23:33:48 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	long long	r;
	long long	x;
	int			s;

	s = 1;
	r = 0;
	while ((*str == 32) || (*str >= 9 && *str <= 13))
		str++;
	if ((*str == '-') || (*str == '+'))
	{
		if (*str == '-')
			s = -1;
		str++;
	}
	while ((*str >= '0') && (*str <= '9'))
	{
		x = r;
		r = r * 10 + (*str - '0') * s;
		str++;
		if ((r > x) && (s < 0))
			return (0);
		if ((r < x) && (s > 0))
			return (-1);
	}
	return (r);
}
