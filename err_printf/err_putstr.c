/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 10:08:24 by ryagoub           #+#    #+#             */
/*   Updated: 2024/05/22 17:30:17 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "err_printf.h"

size_t	ft_strlen1(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

size_t	err_putstr(char *s)
{
	if (s == NULL)
	{
		write (2, "(null)", 6);
		return (6);
	}
	write (2, s, ft_strlen1(s));
	return (ft_strlen1(s));
}
