/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 23:36:16 by mateo             #+#    #+#             */
/*   Updated: 2024/01/03 23:36:16 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (!haystack && (len == 0))
		return (0);
	if (*needle == '\0')
		return ((char *)haystack);
	i = 0;
	while ((haystack[i] != '\0') && (i < len))
	{
		j = 0;
		while ((haystack[i + j] == needle[j]) && (i + j < len))
		{
			if (needle[j + 1] == '\0')
				return ((char *)haystack + i);
			j++;
		}
		i++;
	}
	return (0);
}
