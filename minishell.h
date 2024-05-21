/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 05:43:53 by mateo             #+#    #+#             */
/*   Updated: 2024/05/21 18:12:55 by mateo            ###   ########.fr       */
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

typedef struct s_ast
{
	char	*cmd;
	int		code;
	int		n_args;
	char	**args;
	char	*input;
	char	*output;
	char	*append;
	struct s_ast	*root;
	struct s_ast	*next;
	struct s_ast	*left;
	struct s_ast	*right;
} t_ast;

// parsing.c
int	check_direct(char *line);
int check_quotes(char *line);
int check_pipes(char *line);
int check_all(char *line);
int check_or(char *line);
int check_and(char *line);
int check_op_para(char *line);
int check_close_para(char *line);
int check_single_and(char *line);
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
int		check_quote(int quote, char input);

// parse.c
t_ast	*parse_tokens(t_token **tokens);

// parse_ast_list.c
t_ast	*ast_node_init(void);
int		ast_node_add(t_token **tokens, t_ast **start, t_ast **current);
int		count_args(t_token *tokens);
int		ast_node_append_arg(t_token **tokens, t_ast *current);
int		ast_node_append_misc(t_token **tokens, t_ast *current);
t_ast	*ast_list_new(t_token **tokens);
void	ast_list_print(t_ast *node);
void	ast_list_free(t_ast *node);

// parse_ast_tree.c
t_ast	*ast_tree_new(t_ast **node);
void	ast_tree_print(t_ast *node);

//execute_ast_tree.c
int	execute_ast(t_ast *node);

#endif
