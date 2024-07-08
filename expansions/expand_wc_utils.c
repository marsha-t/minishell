/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wc_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 09:32:47 by ryagoub           #+#    #+#             */
/*   Updated: 2024/07/07 16:36:56 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *ft_strrev(char *str)
{
	char *strrev;
	int i;
	int l;
	i = 0;
	strrev = malloc(sizeof(char)*(ft_strlen(str) + 1));
	if(!strrev)
		return(ft_putstr_fd("minishell: error from strrev in expand_wc",2),NULL);
	l = ft_strlen(str) -1;
	while (l >= 0)
	{
		strrev[i] =str[l];
		l--;
		i++;
	}
	strrev[i] = '\0';
	return(strrev);
}
int	ft_strcmp1(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if(!s1 || !s2)
		return(0);
	while ((s1[i] == s2[i]) && (s1[i] != '\0'))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
int list_size(t_dconts *list)
{
	int i;
	t_dconts *c;
	c = list;
	i = 0;
	while (c)
	{
		i++;
		c = c -> next;
	}
	return(i);
}
