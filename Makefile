NAME = minishell
CFILES =  main.c parsing.c tokenise.c #add files here 
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
