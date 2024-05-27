/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 09:36:22 by ryagoub           #+#    #+#             */
/*   Updated: 2024/05/22 17:33:05 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int		ft_printf(const char *format, ...);
int		ft_putchar(int a);
size_t	ft_putstr(char *s);
int		ft_putnbr(int n);
int		ft_puthexa(unsigned int n);
int		ft_puthexal(unsigned long long n);
int		ft_putunsigned(unsigned int n);

#endif
