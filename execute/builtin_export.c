/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:24:32 by ryagoub           #+#    #+#             */
/*   Updated: 2024/06/09 22:31:31 by ryagoub          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void export(char *s1,char *s2, t_var **envp)
{
	t_var *current;
	// t_var *p;
	t_var *new;

	current = *envp;
	new = malloc(sizeof(t_var));
	if(ft_strcmp(s1, "export") == 0 && s2 )
	{
		if(check_exist(s2,*envp))
		{
			new= check_exist(s2, *envp);
			new ->content = s2;
		}
		else
		{
			*envp = new;
			new -> next = current;
			new ->content = s2;
			if(!(ft_strchr(s2,'=')))
				new-> flag = 1;
		}
	}
}
