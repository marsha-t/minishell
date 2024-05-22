/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nums_of_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 13:42:04 by ryagoub           #+#    #+#             */
/*   Updated: 2024/05/18 13:47:06 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
 int num_of_tokens(t_token *token)
 {
	int i;
	i = 0;
	if(!token)
		return(0);
	while (token)
	{
		i++;
		token = token->next;
	}
	return (i);
 }
