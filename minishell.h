/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 05:43:53 by mateo             #+#    #+#             */
/*   Updated: 2024/05/10 06:54:18 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include "libft/libft.h"

# define TOKEN_TEMP 0
# define TOKEN_CMD 1
# define TOKEN_ARG 2
# define TOKEN_PIPE 3
# define TOKEN_INPUT 4
# define TOKEN_OUTPUT 5
# define TOKEN_APPEND 6
# define TOKEN_HEREDOC 7
# define TOKEN_FILE 8
# define TOKEN_DELIMIT 9

typedef struct	s_token
{
	char			*str;
	int				code;
	struct s_token	*next;
}	t_token;

typedef struct	s_gen
{
	char	*input;
	t_list	*tokens;
}	t_gen;

// parsing.c
int	check_direct(char *line);
int check_quotes(char *line);
int check_pipes(char *line);
int check_all(char *line);

// tokenisation.c
char	*strdup_range(char *start, char *end);
t_token	*new_token(char *str, int code);
void	add_token(t_token **tokens, char *str, int code);
void	tokenise_op(char **input, t_token **tokens);
int		check_quote(char input);
void	tokenise_misc(char **input, t_token **tokens);
int		sort_heredoc(t_token **tokens);
// int	sort_temp_tokens(t_token *tokens);
t_token	*tokenise(char *input);
void	print_token(t_token *tokens);

#endif