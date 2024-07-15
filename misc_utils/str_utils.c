/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 04:03:54 by mateo             #+#    #+#             */
/*   Updated: 2024/07/15 07:56:30 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	ft_strjoin_free joins str as ft_strjoin would except
	- returns s2 if s1 is null
	- frees s1 and s2*/
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*ptr;
	size_t	tot_len;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (free_num(2, s1, s2), NULL);
	tot_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	ptr = (char *)malloc(sizeof(char) * tot_len);
	if (!ptr)
		return (0);
	i = 0;
	j = 0;
	while (s1[j])
		ptr[i++] = s1[j++];
	j = 0;
	while (s2[j])
		ptr[i++] = s2[j++];
	ptr[i] = '\0';
	free_num(2, s1, s2);
	return (ptr);
}

/*	strjoin_num_free joins multiple strings 
	and frees the strings given as arguments
	- if error allocating for strings, the arguments are not freed
	- if string given is NULL, other arguments are not freed
	*/
char	*strjoin_num_free(int num, ...)
{
	va_list	strs;
	char	*temp;
	char	*arg;

	va_start(strs, num);
	arg = va_arg(strs, char *);
	if (!arg)
	{
		while (--num)
		{
			arg = va_arg(strs, char *);
			free(arg);
		}
		return (err_printf("minishell: malloc error: ft_strdup\n"), NULL);
	}
	temp = ft_strdup(arg);
	if (!temp)
		return (err_printf("minishell: malloc error: strjoin temp\n"), NULL);
	free(arg);
	while (--num)
	{
		arg = va_arg(strs, char *);
		if (!arg)
		{
			while (--num)
			{
				arg = va_arg(strs, char *);
				free(arg);
			}
			return (err_printf("minishell: malloc error: ft_strdup\n"), NULL);
		}
		temp = ft_strjoin_free(temp, arg);
		if (!temp)
			return (err_printf("minishell: malloc error: strjoin temp\n"), NULL);
	}
	va_end(strs);
	return (temp);
}

/*	ft_strcmp compares two strings
	- returns non-zero if they are different;
	- returns 0 if they are the same */
int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] == s2[i]) && (s1[i] != '\0'))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
