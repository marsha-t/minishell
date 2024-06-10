# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ryagoub <ryagoub@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 15:41:17 by ryagoub           #+#    #+#              #
#    Updated: 2024/06/08 14:26:46 by ryagoub          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a
CFILES = ft_putnbr.c ft_putchar.c ft_putstr.c ft_printf.c ft_puthexa.c ft_puthexal.c ft_putunsigned.c
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
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
