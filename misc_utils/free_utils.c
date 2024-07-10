/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 03:59:22 by mateo             #+#    #+#             */
/*   Updated: 2024/07/09 04:02:46 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	free_num frees an arbitrary number of pointers
	num = number of pointers to be freed*/
void	free_num(int num, ...)
{
	va_list	args;

	va_start(args, num);
	while (num--)
	{
		free(va_arg(args, void *));
	}
	va_end(args);
}

/*	free_char_dp frees a char double pointer that is null-terminated*/
void	free_char_dp(char **dp)
{
	int	i;

	i = 0;
	while (dp[i])
	{
		free(dp[i]);
		i++;
	}
	free(dp);
}

void	free_safe(void *pointer)
{
	free(pointer);
	pointer = NULL;
}
