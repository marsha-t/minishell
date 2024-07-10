/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 09:35:00 by ryagoub           #+#    #+#             */
/*   Updated: 2024/05/22 17:34:16 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"


static char	*ft_strchr1(const char *s, int c)
{
	int		i;
	char	*str;

	i = 0;
	str = (char *)s;
	while (str[i] != '\0')
	{
		if (str[i] == (char)c)
		{
			return (str + i);
		}
		i++;
	}
	if (str[i] == (char)c)
	{
		return (str + i);
	}
	return (0);
}

static int	output(const char*format, int i, va_list args)
{
	int	outputlength;

	outputlength = 0;
	while (ft_strchr1("cspdiuxX%", format[i]) == NULL)
		i++;
	if (format[i] == 'c')
		outputlength += ft_putchar(va_arg(args, int));
	else if (format[i] == 's')
		outputlength += ft_putstr(va_arg(args, char *));
	else if (format[i] == 'p')
		outputlength += ft_putstr("0x")
			+ ft_puthexal((unsigned long long) va_arg(args, void*));
	else if (format[i] == 'i' || format[i] == 'd')
		outputlength += ft_putnbr(va_arg(args, int));
	else if (format[i] == 'u')
		outputlength += ft_putunsigned(va_arg(args, unsigned int));
	else if (format[i] == 'X')
		outputlength += ft_puthexa(va_arg(args, unsigned int));
	else if (format[i] == 'x')
		outputlength += ft_puthexal(va_arg(args, unsigned int));
	else if (format[i] == '%')
		outputlength += ft_putchar('%');
	i++;
	return (outputlength);
}

int	ft_printf(const char *format, ...)
{
	int		i;
	va_list	args;
	int		outputlength;

	i = 0;
	outputlength = 0;
	va_start(args, format);
	while (format[i] != '\0')
	{
		if (format[i] == '%')
		{
			i++;
			outputlength += output(format, i, args);
			i++;
		}
		else
		{
			while (format[i] != '\0' && format[i] != '%')
			{
				outputlength += ft_putchar(format[i]);
				i++;
			}
		}
	}
	return (va_end(args), outputlength);
}
