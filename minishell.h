/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 05:43:53 by mateo             #+#    #+#             */
/*   Updated: 2024/07/22 10:53:12 by ryagoub          ###   ########.fr       */
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
# include <fcntl.h>
# include <dirent.h>
# include <sys/stat.h>
# include "libft/libft.h"
# include "printf/ft_printf.h"
#include <sys/wait.h>
#include <time.h>
#include <string.h>
# include <signal.h>
#include "global.h"
# include "err_printf/err_printf.h"

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

typedef struct s_pipe_info
{
	int			pipe_count;
	int        **pipes;
	int			*pid;
}t_pipe_info;


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

typedef struct	s_wc
{
	t_dconts	*matched;
	char		*cmd;
}	t_wc;

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
	struct s_ast	*pipe;

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
	t_ast	*ast_list;
	t_ast	*root;
	int		pipe_data;
	pid_t		*pid; // MT: new code
	int	exit_status;
	int	exit_shell;
	int  old_read_fd;
	int file_err;
}	t_shell;

/*****************************************************************************/
/*	initialise																 */
/*****************************************************************************/
// shell.c
t_shell	*init_shell(char **envp);
void	free_after_command(t_shell *shell);
void	free_shell(t_shell *shell);

// env_var.c
int create_node(t_var **v, char *str, int flag);
t_var *create_list(char **envp);
void	free_var_list(t_var *var);

/*****************************************************************************/
/*	check																	 */
/*****************************************************************************/
// syntax_err1.c
int check_and(char *line);
int	check_direct(char *line);
int check_quotes(char *line);
int check_pipes(char *line);
int check_all(char *line);

// syntax_err2.c
int check_or(char *line);

int check_op_para(char *line);
int check_close_para(char *line);
int	check_empty_para(char *line);

// syntax_err_utils1.c
int	is_direct_double(char a, char b);
int	only_whitespace_left(char *line, int i);
int	pipe_mid(int i, char *line);
int	and_mid(int i, char *line);
int	or_mid(int i, char *line);

/*syntax_err_utils2.c*/
void	skip_quoted(char *line, int *i);
int	check_op_para_count(char *line, int *i, int *op_count);

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
// void	free_tokens(t_token *tokens);
void	free_tokens_null(t_token **tokens);

// tokenise_misc_utils.c
char	*strdup_range(char *start, char *end);
int		is_file_op(int code);
int		is_cmdorder_op(int code);
int		check_quote(int quote, char input);

/*****************************************************************************/
/*	parse																	 */
/*****************************************************************************/
// parse.c
// t_ast	*parse_tokens(t_token **tokens);
int	parse_tokens(t_shell *shell);

// parse_ast_list.c
t_ast	*ast_node_init(void);
int		ast_node_add(t_token **tokens, t_ast **start, t_ast **current);
int		count_args(t_token *tokens);
int	ast_node_append_cmd(t_token **tokens, t_ast *current);
int		ast_node_append_arg(t_token **tokens, t_ast *current);
int	ast_node_append_misc(t_token **tokens, t_ast **start, t_ast **current);
t_ast	*ast_list_new(t_token **tokens);
void	file_list_free(t_file *file);
void	ast_list_free(t_ast *node);

// create_redir_lists.c
int create_in_list(t_token *token, t_ast **node);
int create_heredoc_list(t_token *token, t_ast **node);
int create_output_append_list(int code, t_token *token, t_ast **node);

// parse_ast_tree.c
int	is_logical_op(int code);
t_ast	*ast_pipeline_new(t_ast **node);
t_ast	*ast_tree_new(t_ast **node);

/*****************************************************************************/
/*	expand																	 */
/*****************************************************************************/
// quote_remove.c
char	*remove_quote_join(char *str, char *before_quote, int *start, int *i);
char	*remove_quote_str(char *str);
int	remove_quote_file(t_file *file);
int	remove_quote_node(t_ast *node);

// expand_var.c
char	*ft_strjoin2(char const *s1, char const *s2);
char  *expand_var(char *var, t_var *env);
char *join_expand(char *temp, char *var, char *str, int i);
char	*split_expand_join(char *str, int i, t_shell *shell);
char	*expand_str(char *str, t_shell *shell);
int	file_list_check_var(t_file *file, t_shell *shell);
int	check_var_expansion(t_ast *node, t_shell *shell);

// expand_var_utils2.c
int	is_valid_varstart(char c);
int	is_valid_varchar(char c);
int	is_quote(char c);
int contain_var(char *str);

// check_wc.c
int contain_wc(char *str);
int	file_list_check_wc(t_ast *node, t_shell *shell, int code);
int	redir_check_wc(t_ast *node, t_shell *shell);
int	arg_check_wc(t_ast *node, t_shell *shell);
int	check_wc_expansion(t_ast *node, t_shell *shell);

// expand_wc.c
void	rm_dot_dconts(t_dconts **list);

int expand_wc_setup(t_dconts **matched_list, char *pattern, t_dconts **list);
int	expand_wc_cmd(t_dconts **list, t_ast *node);
int	expand_wc_arg(t_dconts **list, t_ast *node, char *pattern);
int	expand_wc_file(t_shell *shell, t_ast *node, char *pattern, int code);

// expand_wc_utils.c
int	check_directory(char *directory);
int	match_dir_end(char *dir, t_dconts **matched);
int	match_dcont(char *dir, char *pattern, t_dconts **dcont);
int	match_dir_matched(t_wc *wc_info, char *next_slash, char *new_directory);
int	match_dir_while(char *dir, char *pattern, char *next_slash, t_wc *wc_info);
int	match_dir(t_wc *wc_info, char *slash, char *dir);
int	count_matches(t_dconts *matched);
t_list	*create_arg(char *arg_str);
int	matched_to_arg(t_dconts *matched, t_list **new_args, t_list **end_args);
int add_matched_to_arg(t_dconts *matched, t_list **arg, t_ast *node);
int	init_wc(t_wc **wc_info, char *cmd);
void	free_wc_info(t_wc *wc_info);

// match_wc.c
int	match_pattern_str(char *pattern, char *str);
int	match_pattern_list(char *pattern, t_dconts *list, t_dconts **matched_list);

// expand_wc_conts_list.c
int create_conts_node(char *str, t_dconts **list);
void    order_conts_list(t_dconts **list);
t_dconts *create_conts_list(char *directory);
void	free_conts_list(t_dconts *list);

/*****************************************************************************/
/*	execute																	 */
/*****************************************************************************/
// execute_ast_tree.c
int	execute_cmd_builtin(t_ast *node, t_shell *shell);
int	execute_cmd_others(t_ast *node, t_shell *shell);

int	cmd_only_quote(char *cmd);
int	execute_cmd_node(t_ast *node, t_shell *shell);
int		execute_ast(t_ast *node, t_shell *shell);

// input_files.c
int		get_infile(t_ast *node, t_shell *shell);
 int dup_input(t_shell *shell, t_ast *node, int fd);


// output_files.c
int  open_files (t_ast *node, t_shell *shell);
int get_outfile(t_ast *node, t_shell *shell);
int dup_output(t_shell *shell, t_ast *node, int o_fd);
// void close_files(t_ast *node);

// here_doc.c
int open_hd_file(t_file *current, char *uniq_file, t_shell *shell);
int get_docs(t_ast *node, t_shell *shell);

// handling signals
void  control_signals(void);

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
void	exit_shell(t_shell *shell, int exit_status);
int	builtin_exit(t_ast *node, t_shell *shell);

// builtin_export.c
void print_export(t_var *envp);
t_var *check_exist(char *word, t_var *list);
int builtin_export(t_ast *node, t_shell *shell);

// builtin_pwd.c
char	*ft_getcwd(t_shell *shell);
int		update_pwd(char *dir, t_shell *shell);
int		builtin_pwd(t_ast *node, t_shell *shell);

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
int		count_env(t_var *env);
char	**envp_array(t_var *env);
char	**argv_array(t_ast *node);
char 	*get_filepath(char *cmd, int *exit_status, t_shell *shell);
int		run_external(t_ast *node, t_shell *shell);

// execute_external_utils.c
int		check_filepath(char *cmd, t_shell *shell);
int		has_current_wd(char *path);
char	*add_current_wd(char *path, int i, t_shell *shell);
char	*find_cmd(char *cmd, int *exit_status, t_shell *shell);

/*****************************************************************************/
/*	misc utils																 */
/*****************************************************************************/
// free_utils.c
void	free_num(int num, ...);
void	free_char_dp(char **dp);
void	free_safe(void **pointer);

// str_utils.c
char	*ft_strjoin_free(char *s1, char *s2);
char	*strjoin_num_free(int num, ...);
int		ft_strcmp(const char *s1, const char *s2);

// print_utils.c
void	print_var_list(t_var *var);
void	print_tokens(t_token *tokens);
void	file_list_print(t_file *file, int type);
void	ast_list_print(t_ast *node);

// print_utils2.c
void	ast_pipeline_print(t_ast *node);
void	ast_tree_print(t_ast *node);
void	print_conts_list(t_dconts *list);

// error_utils.c
int    err_syscall(t_shell *shell, char *cmd);
int err_syntax(char *token, int n);

#endif
