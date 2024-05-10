/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 23:36:22 by mateo             #+#    #+#             */
/*   Updated: 2024/01/03 23:36:22 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*temp;

	temp = 0;
	while (*s)
	{
		if (*s == (char)c)
			temp = s;
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	else
		return ((char *)temp);
}
