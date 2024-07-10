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

#ifndef ERR_PRINTF_H
# define ERR_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int		err_printf(const char *format, ...);
int		err_putchar(int a);
size_t	err_putstr(char *s);
int		err_putnbr(int n);
int		err_puthexa(unsigned int n);
int		err_puthexal(unsigned long long n);
int		err_putunsigned(unsigned int n);

#endif
