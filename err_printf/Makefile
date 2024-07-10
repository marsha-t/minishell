# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 15:41:17 by ryagoub           #+#    #+#              #
#    Updated: 2024/06/17 12:43:45 by ryagoub          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = err_printf.a
CFILES = err_putnbr.c err_putchar.c err_putstr.c err_printf.c err_puthexa.c err_puthexal.c err_putunsigned.c
CFLAGS = -Wall -Wextra -Werror -g3
OFILES = $(CFILES:.c=.o)

all: $(NAME)

$(NAME): $(OFILES)
	ar rcs $(NAME) $(OFILES)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OFILES)

fclean: clean
	$(RM) $(NAME)

re: fclean all
