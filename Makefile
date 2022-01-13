# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jzoltan <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/22 05:56:00 by jzoltan           #+#    #+#              #
#    Updated: 2022/01/11 17:29:27 by jzoltan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libftprintf.a

SRCS=ft_printf.c

OBJS=ft_printf.o

CC=cc

CFLAGS=-Werror -Wextra -Wall

ARCHIVE=ar rc

all: $(NAME)

ft_printf.o: ft_printf.h

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(ARCHIVE) $(NAME) $(OBJS)

clean:
	rm -f $(NAME)

fclean: clean
	rm -f $(OBJS)
	rm -f $(BONUS_OBJS)

bonus: all

re: fclean all

.PHONY: all clean fclean re bonus
