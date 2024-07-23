NAME = minishell
INITIALISE_DIR = initialise
INITIALISE_CFILES = shell.c env_var.c
CHECK_DIR = check
CHECK_CFILES = syntax_err1.c syntax_err2.c syntax_err_utils1.c syntax_err_utils2.c
TOKENISE_DIR = tokenise
TOKENISE_CFILES = tokenise1.c tokenise2.c tokenise_token_utils.c tokenise_misc_utils.c
PARSE_DIR = parse
PARSE_CFILES = parse.c parse_ast_list1.c parse_ast_list2.c parse_ast_list_utils.c free_ast_list.c parse_ast_tree.c parse_ast_pipeline.c create_redir_lists.c
EXPANSIONS_DIR = expansions
EXPANSIONS_CFILES = expand_var1.c expand_var2.c expand_var3.c expand_var_utils1.c expand_var_utils2.c check_wc.c expand_wc.c expand_wc_utils.c match_wc.c expand_wc_conts_list.c quote_remove1.c quote_remove2.c
EXECUTE_DIR = execute
EXECUTE_CFILES = execute_ast_tree.c  execute_external.c execute_external_utils.c \
	builtin_echo.c builtin_exit.c  builtin_pwd.c input_files.c output_files.c here_doc.c handle_signals.c \
	execute_assign.c builtin_cd.c builtin_env.c builtin_export.c builtin_unset.c
MISC_UTILS_DIR = misc_utils
MISC_UTILS_CFILES = free_utils.c str_utils.c print_utils.c print_utils2.c error_utils.c

CFILES =  main.c  	global.c\
		$(addprefix $(INITIALISE_DIR)/, $(INITIALISE_CFILES)) \
		$(addprefix $(CHECK_DIR)/, $(CHECK_CFILES)) \
		$(addprefix $(TOKENISE_DIR)/, $(TOKENISE_CFILES)) \
		$(addprefix $(PARSE_DIR)/, $(PARSE_CFILES)) \
		$(addprefix $(EXPANSIONS_DIR)/, $(EXPANSIONS_CFILES)) \
		$(addprefix $(EXECUTE_DIR)/, $(EXECUTE_CFILES)) \
		$(addprefix $(MISC_UTILS_DIR)/, $(MISC_UTILS_CFILES))

OFILES = ${CFILES:.c=.o}

LIBFT = libft.a
LIBFT_DIR = libft

FT_PRINTF =	libftprintf.a
FT_PRINTF_DIR = printf

CFLAGS = -Wall -Wextra -Werror

ERR_PRINTF = err_printf.a
ERR_PRINTF_DIR = err_printf

all: $(NAME)

%o: %c
	cc ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME): $(OFILES) $(LIBFT_DIR)/$(LIBFT) $(ERR_PRINTF_DIR)/$(ERR_PRINTF) $(FT_PRINTF_DIR)/$(FT_PRINTF)
	cc $(CFLAGS) $(OFILES) $(LIBFT_DIR)/$(LIBFT) $(FT_PRINTF_DIR)/$(FT_PRINTF) $(ERR_PRINTF_DIR)/$(ERR_PRINTF) -o $(NAME) -lreadline

$(LIBFT_DIR)/$(LIBFT):
	make bonus -C $(LIBFT_DIR)

$(FT_PRINTF_DIR)/$(FT_PRINTF):
	make -C $(FT_PRINTF_DIR)

$(ERR_PRINTF_DIR)/$(ERR_PRINTF):
	make -C $(ERR_PRINTF_DIR)

clean:
	make clean -C $(LIBFT_DIR)
	make clean -C $(FT_PRINTF_DIR)
	make clean -C $(ERR_PRINTF_DIR)
	rm -f $(OFILES)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	make fclean -C $(FT_PRINTF_DIR)
	make fclean -C $(ERR_PRINTF_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

