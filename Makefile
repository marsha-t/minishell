NAME = minishell
CFILES =  #add files here 
OFILES = ${CFILES:.c=.o}
CFLAGS = -Wall -Wextra -Werror 

all: $(NAME)

%o: %c 
	cc ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME): $(OFILES)
	cc $(CFLAGS) $(OFILES) -o $(NAME) -lreadline

clean:
	rm -f $(OFILES) 

fclean: clean
	rm -f $(NAME) 

re: fclean all

.PHONY: all clean fclean re
