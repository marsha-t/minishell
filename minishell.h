/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 05:43:53 by mateo             #+#    #+#             */
/*   Updated: 2024/06/03 13:04:06 by mateo            ###   ########.fr       */
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
# include "printf/ft_printf.h"

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
# define TOKEN_IONUM 13

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
	int		n_input;
	int		*io_input;	// io_input[0] will contain io number for first input redirection
	char	**file_input; // file_input[0] will contain file for first input redirection
	int		n_output_append; // number of output and append redirections 
	int		*io_output_append; // io_output_append[0] will contain io number for first output/append redirection
	int		*is_append;// 0 if output; 1 if append
	char	**file_output_append; // file_output_append[0] will contain file for first output/append redirection
	struct s_ast	*root;
	struct s_ast	*next;
	struct s_ast	*left;
	struct s_ast	*right;
} t_ast;

// environment variables
typedef struct s_var
{
	char	*key;
	char	*value;
	int		flag;
	int		env;
	struct s_var *next;
} t_var;

// shell structure to hold all variables/data
typedef struct s_shell
{
	t_var	*var_list;
	char	*line;
	t_token	*tokens;
	t_ast	*root;
}	t_shell;

// shell.c
void	free_safe(void *pointer);
t_shell	*init_shell(char **envp);
void	free_shell(t_shell *shell);

// env_var.c
int create_node(t_var **v, char *str);
t_var *create_list(char **envp);
void	free_var_list(t_var *var);

// syntax_err.c
int	check_direct(char *line);
int check_quotes(char *line);
int check_pipes(char *line);
int check_all(char *line);

// syntax_err_bo.c
int check_and(char *line);
int check_or(char *line);
int check_op_para(char *line);
int check_close_para(char *line);

// syntax_bo_utils.c
int	pipe_mid(int i, char *line);
int	and_mid(int i, char *line);
int	or_mid(int i, char *line);

// tokenise.c
int		tokenise_op(char **input, t_token **tokens);
int		tokenise_misc(char **input, t_token **tokens);
int		check_syntax_tokens(t_token *tokens);
int		sort_temp_tokens(t_token *tokens);
t_token	*tokenise(char *input);

// tokenise_token_utils.c
t_token	*new_token(char *str, int code);
int		add_token(t_token **tokens, char *str, int code);
void	free_tokens(t_token *tokens);
void	print_tokens(t_token *tokens);

// tokenise_misc_utils.c
int		ft_strcmp(const char *s1, const char *s2);
char	*strdup_range(char *start, char *end);
int		is_file_op(int code);
int		is_cmdorder_op(int code);
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
int		execute_ast(t_ast *node);

// env_var_utils.c
char  *expand_var(t_var **env,char *key);
int key_len(char *str);
char *return_key(char *str);
char  *search_for_key(char *key,char *str);
int var_length(char *str);
int ft_strlen_b_$(char *str);
char *value(char *str , t_var **envp);
int is_var(char *str);

// environment_vars_op.c
void env_ops(t_var **list, t_token *token);

// var_modif.c 
void print_export(char *str, t_var **envp);
void print_envp(char *str, t_var **envp);
void export(char *s1,char *s2, t_var **envp);
void unset(char *s1,char *s2, t_var **envp);

// unset_and_export_utils.c
t_var *check_exist(char *word, t_var *list);
t_var *search_for_node(char *s2, t_var **list);

#endif
