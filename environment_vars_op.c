/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_vars_op.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 14:53:29 by ryagoub           #+#    #+#             */
/*   Updated: 2024/05/22 12:51:43 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void env_ops(t_var **list, t_token *token)
{
	while(token!= NULL)
	{
		if(token -> code == 1)
		{
			print_envp(token->str,list);
			if(token -> next)
			{
				export(token->str,token-> next->str, list);
				unset(token->str,token-> next->str, list);
			}
			else if(!token -> next)
				print_export(token -> str, list);
		}
		if(token -> code == 2 && is_var(token->str)== 1)
			token->str = value(token->str,list);
		token = token -> next;
	}
	free(token);
}
