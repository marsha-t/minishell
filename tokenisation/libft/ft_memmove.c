/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 11:17:01 by mateo             #+#    #+#             */
/*   Updated: 2024/01/03 23:35:05 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	void	*dst_start;

	dst_start = dst;
	if (src == dst)
		return (dst);
	else if (src > dst)
	{
		while (len--)
		{
			*(char *)dst = *(char *)src;
			dst++;
			src++;
		}
	}
	else
	{
		while (len--)
			*(char *)(dst + len) = *(char *)(src + len);
	}
	return (dst_start);
}
