/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 05:43:53 by mateo             #+#    #+#             */
/*   Updated: 2024/06/28 01:12:24 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <errno.h>
# include <stdarg.h>
#include <fcntl.h>
# include <dirent.h>
#include <sys/stat.h>
# include "libft/libft.h"
# include "printf/ft_printf.h"
#include <sys/wait.h>
#include <time.h>

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

typedef struct	s_file
{
	char			*file_name;
	int				flag;
	int 			fd;
	struct s_file	*next;
} t_file;

// struct to save the contents of the current directory
// typedef struct s_dconts
// {
// 	char *cont_name;
// 	struct s_dconts *next;
// } t_dconts;

// typedef struct s_list
// {
// 	char *arg_str;
// 	struct s_list*next;
// } t_list;

// struct to save the contents of the current directory
typedef struct s_dconts
{
	char *cont_name;
	struct s_dconts *next;
} t_dconts;

typedef struct s_ast
{
	char	*cmd;
	int		code;
	int		n_args;
	int tmp_stdin_fd;
	int tmp_stdout_fd;
   	t_list *args;
	t_file *input_list;
	t_file *output_list;
	t_file *heredoc_list;
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
	t_dconts *directory_contents;
	char	*line;
	t_token	*tokens;
	t_ast	*root;
	int	exit_status;
	int	exit_shell;
}	t_shell;

/*****************************************************************************/
/*	initialise																 */
/*****************************************************************************/
// shell.c
void	free_safe(void *pointer);
t_shell	*init_shell(char **envp);
void	free_shell(t_shell *shell);

// env_var.c
int create_node(t_var **v, char *str, int flag);
t_var *create_list(char **envp);
void	free_var_list(t_var *var);

/*****************************************************************************/
/*	check																	 */
/*****************************************************************************/
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

/*****************************************************************************/
/*	tokenise																 */
/*****************************************************************************/
// tokenise.c
int		tokenise_op(char **input, t_token **tokens);
int		tokenise_misc(char **input, t_token **tokens);
int		check_syntax_tokens(t_token *tokens);
void	sort_temp_tokens(t_token *tokens);
// t_token	*tokenise(char *input);
int	tokenise(char *input, t_token **tokens);

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

/*****************************************************************************/
/*	parse																	 */
/*****************************************************************************/
// parse.c
t_ast	*parse_tokens(t_token **tokens);

// parse_ast_list.c
t_ast	*ast_node_init(void);
int		ast_node_add(t_token **tokens, t_ast **start, t_ast **current);
int		count_args(t_token *tokens);
int	ast_node_append_cmd(t_token **tokens, t_ast *current);
int		ast_node_append_arg(t_token **tokens, t_ast *current);
int	ast_node_append_misc(t_token **tokens, t_ast **start, t_ast **current);
t_ast	*ast_list_new(t_token **tokens);
void	ast_list_print(t_ast *node);
void	ast_list_free(t_ast *node);

// create_redir_lists.c
int create_in_list(t_token *token, t_ast **node);
int create_heredoc_list(t_token *token, t_ast **node);
int create_output_append_list(int code, t_token *token, t_ast **node);

// parse_ast_tree.c
t_ast	*ast_tree_new(t_ast **node);
void	ast_tree_print(t_ast *node);

/*****************************************************************************/
/*	expand																	 */
/*****************************************************************************/
// quote_remove.c
char	*ft_strjoin_free(char *s1, char *s2);
char	*strjoin_num_free(int num, ...);
char	*remove_quote_join(char *str, char *before_quote, int *start, int *i);
char	*remove_quote_str(char *str);
int	remove_quote_file(t_file *file);
int	remove_quote_node(t_ast *node);

// expand_var.c
char	*ft_strjoin2(char const *s1, char const *s2);
char  *expand_var(char *var, t_var *env);
char *join_expand(char *temp, char *var, char *str, int i);
char	*split_expand_join(char *str, int i, t_var *list);
char	*expand_str(char *str, t_var *list);

// expand_var_utils.c
int	is_valid_varstart(char c);
int	is_valid_varchar(char c);
int	is_quote(char c);
int contain_var(char *str);

/*****************************************************************************/
/*	execute																	 */
/*****************************************************************************/
// execute_ast_tree.c
int	execute_cmd(t_ast *node, t_shell *shell);
int	cmd_only_quote(char *cmd);
int	execute_cmd_node(t_ast *node, t_shell *shell);
int		execute_ast(t_ast *node, t_shell *shell);
// redirection
int		get_infile(t_ast *node);
int get_outfile(t_ast *node);
void close_files(t_ast *node);

// builtin_cd.c
int	builtin_cd(t_ast *node, t_shell *shell);

// builtin_echo.c
int	is_newline_arg(char *arg);
int	builtin_echo(t_ast *node);

// builtin_env.c
void print_envp(t_var *envp);
int	builtin_env(t_ast *node, t_shell *shell);

// builtin_exit.c
unsigned long long	ft_atoi_ull(char *str);
int check_ll_limit(char *str, int sign);
int	check_exit_arg(char *str);
int	get_exit_status(char *str);
void	exit_shell(t_shell *shell);
int	builtin_exit(t_ast *node, t_shell *shell);

// builtin_export.c
void print_export(t_var *envp);
t_var *check_exist(char *word, t_var *list);
int builtin_export(t_ast *node, t_shell *shell);

// builtin_pwd.c
char	*ft_getcwd(t_shell *shell);
int	update_pwd(char *dir, t_shell *shell);
int	builtin_pwd(t_ast *node, t_shell *shell);


// builtin_unset.c
int	builtin_unset(t_ast *node, t_shell *shell);

// execute_assign.c
int	valid_varname(char *name);
int	create_key_value(char *str, char **equal, char **key, char **value);
int	create_node_normal(t_var **v, char *key, char *value);
int	run_assign_str(char *cmd, t_shell *shell);
int	check_assign_varname(t_ast *node, t_shell *shell);
int	run_assign_cmd(t_ast *node, t_shell *shell);
int	run_assign(t_ast *node, t_shell *shell);

// execute_external.c
int	count_env(t_var *env);
char	**envp_array(t_var *env);
char	**argv_array(t_ast *node);
char *get_filepath(char *cmd, int *exit_status, t_shell *shell);
int	run_external(t_ast *node, t_shell *shell);

// execute_external_utils.c
void	free_num(int num, ...); // move to basic utils?
int	check_filepath(char *cmd, t_shell *shell);
void	free_char_dp(char **dp);
int	has_current_wd(char *path);
char *add_current_wd(char *path, int i, t_shell *shell);
char	*find_cmd(char *cmd, int *exit_status, t_shell *shell);

/*****************************************************************************/
/*	misc																	 */
/*****************************************************************************/
// env_var_utils.c
// char  *expand_var(char **var, t_var **env);
char *expand_str(char *str, t_var *list);
int key_len(char *str);
char *return_key(char *str);
char  *search_for_key(char *key,char *str);
int var_length(char *str);
int ft_strlen_b_$(char *str);
int contain_var(char *str);

t_dconts *create_conts_list(void);
char *ft_strrev(char *str);
int	ft_strcmp1(const char *s1, const char *s2);
t_dconts *expand_wildcard(char *str, t_dconts *conts_list);
int list_size(t_dconts *list);




#endif
