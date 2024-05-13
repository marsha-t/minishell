/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 05:43:53 by mateo             #+#    #+#             */
/*   Updated: 2024/05/13 14:04:17 by mateo            ###   ########.fr       */
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
# define TOKEN_OR 9
# define TOKEN_AND 10
# define TOKEN_OBRACKET 11
# define TOKEN_CBRACKET 12

typedef struct	s_token
{
	char			*str;
	int				code;
	struct s_token	*next;
}	t_token;

// parsing.c
int	check_direct(char *line);
int check_quotes(char *line);
int check_pipes(char *line);
int check_all(char *line);
int check_or(char *line);
int check_and(char *line);
int check_op_para(char *line);
int check_close_para(char *line);

// tokenisation.c
int		tokenise_op(char **input, t_token **tokens);
int		tokenise_misc(char **input, t_token **tokens);
int		sort_temp_tokens(t_token *tokens);
t_token	*tokenise(char *input);
void	print_token(t_token *tokens); // to remove?

// tokenise_token_utils.c
t_token	*new_token(char *str, int code);
int		add_token(t_token **tokens, char *str, int code);
void	free_tokens(t_token *tokens);
void	print_tokens(t_token *tokens);

// tokenise_misc_utils.c
int		ft_strcmp(const char *s1, const char *s2);
char	*strdup_range(char *start, char *end);
int	is_file_op(int code);
int	is_cmdorder_op(int code);
int		check_quote(char input);

#endif