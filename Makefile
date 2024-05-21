NAME = minishell
CFILES =  main.c \
		syntax_err.c syntax_err_bo.c invalid_chars.c \
		tokenise.c tokenise_token_utils.c tokenise_misc_utils.c \
		parse.c parse_ast_list.c parse_ast_tree.c #add files here
OFILES = ${CFILES:.c=.o}

LIBFT = libft.a
LIBFT_DIR = libft

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

%o: %c
	cc ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME): $(OFILES) $(LIBFT)
	cc $(CFLAGS) $(OFILES) $(LIBFT_DIR)/$(LIBFT) -o $(NAME) -lreadline

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(OFILES)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
