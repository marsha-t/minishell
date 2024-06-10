/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 23:35:32 by mateo             #+#    #+#             */
/*   Updated: 2024/06/08 15:12:29 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char		*ptr;
	size_t		n;
	int			i;
	
	n = ft_strlen(src);
	ptr = (char *)malloc(sizeof(char) * (n + 1));
	if (!ptr)
		return (0);
	i = -1;
	while (src[++i] != '\0')
		ptr[i] = src[i];
	ptr[i] = '\0';
	return (ptr);
}
