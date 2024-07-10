/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthexa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 19:03:56 by ryagoub           #+#    #+#             */
/*   Updated: 2023/11/29 17:29:36 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "err_printf.h"

static int	lengthhex(unsigned int n)
{
	int	i;

	i = 0;
	if (n == 0)
		i = 1;
	while (n != 0)
	{
		n /= 16;
		i++;
	}
	return (i);
}

int	err_puthexa(unsigned int n)
{
	int	l;

	l = lengthhex(n);
	if (n < 10)
	{
		err_putchar(n + '0');
	}
	else if (n == 10)
		err_putchar('A');
	else if (n == 11)
		err_putchar('B');
	else if (n == 12)
		err_putchar('C');
	else if (n == 13)
		err_putchar('D');
	else if (n == 14)
		err_putchar('E');
	else if (n == 15)
		err_putchar('F');
	else
	{
		err_puthexa(n / 16);
		err_puthexa(n % 16);
	}
	return (l);
}
