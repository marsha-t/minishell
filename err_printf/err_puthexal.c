/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthexal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 10:48:32 by ryagoub           #+#    #+#             */
/*   Updated: 2023/11/29 17:27:03 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "err_printf.h"

static int	lengthl(unsigned long long n)
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

int	err_puthexal(unsigned long long n)
{
	int	l;

	l = lengthl(n);
	if (n < 10)
	{
		err_putchar(n + '0');
	}
	else if (n == 10)
		err_putchar('a');
	else if (n == 11)
		err_putchar('b');
	else if (n == 12)
		err_putchar('c');
	else if (n == 13)
		err_putchar('d');
	else if (n == 14)
		err_putchar('e');
	else if (n == 15)
		err_putchar('f');
	else
	{
		err_puthexal(n / 16);
		err_puthexal(n % 16);
	}
	return (l);
}
