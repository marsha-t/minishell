/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_vars_op.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 14:53:29 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/03 06:19:35 by mateo            ###   ########.fr       */
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
		
		token = token -> next;
	}
	free(token);
}
