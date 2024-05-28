NAME = minishell
TOKENISE_DIR = tokenise
TOKENISE_CFILES = tokenise.c tokenise_token_utils.c tokenise_misc_utils.c \
					tokenise_redir_tokens.c tokenise_redir_utils.c 
PARSE_DIR = parse
PARSE_CFILES = parse.c parse_ast_list.c parse_ast_tree.c 
EXECUTE_DIR = execute
EXECUTE_CFILES = execute_ast_tree.c

CFILES =  main.c \
		syntax_err.c syntax_err_bo.c syntax_bo_utils.c invalid_chars.c \
		$(addprefix $(TOKENISE_DIR)/, $(TOKENISE_CFILES)) \
     env_variables.c env_var_utils.c var_modif.c nums_of_tokens.c environment_vars_op.c unset_and_export_utils.c		
		# $(addprefix $(PARSE_DIR)/, $(PARSE_CFILES)) \
		# $(addprefix $(EXECUTE_DIR)/, $(EXECUTE_CFILES)) \

OFILES = ${CFILES:.c=.o}

LIBFT = libft.a
LIBFT_DIR = libft

FT_PRINTF =	libftprintf.a
FT_PRINTF_DIR = printf

CFLAGS = -Wall -Wextra -Werror -g3

all: $(NAME)

%o: %c
	cc ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME): $(OFILES) $(LIBFT) $(FT_PRINTF)
	cc $(CFLAGS) $(OFILES) $(LIBFT_DIR)/$(LIBFT) $(FT_PRINTF_DIR)/$(FT_PRINTF) -o $(NAME) -lreadline

$(LIBFT):
	make -C $(LIBFT_DIR)

$(FT_PRINTF):
	make -C $(FT_PRINTF_DIR)

clean:
	make clean -C $(LIBFT_DIR)
	make clean -C $(FT_PRINTF_DIR)
	rm -f $(OFILES)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	make fclean -C $(FT_PRINTF_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
