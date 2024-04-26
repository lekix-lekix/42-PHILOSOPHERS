# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/26 17:19:05 by kipouliq          #+#    #+#              #
#    Updated: 2024/04/25 16:39:35 by kipouliq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

SRC = ./srcs/main.c \
	./srcs/ft_itoa.c \
	./srcs/args_checking.c \
	./srcs/utils.c \
	./srcs/utils_2.c \
	./srcs/monitors.c \
	./srcs/init_data.c \
	./srcs/free_destroy.c \
	./srcs/routines.c \
	./srcs/routines_bis.c

OBJ = $(SRC:.c=.o)

CC = cc

FLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) -g3 $(OBJ) -o $(NAME) -g3

%.o:%.c
	$(CC) $(FLAGS) -c $< -o $@ -g3

clean :
	rm -f ./srcs/*o

san: $(OBJ)
	$(CC) $(FLAGS) -fsanitize=thread -g3 $(OBJ) -o $(NAME) 

fclean : clean
	rm -f $(NAME)
	rm -f ./philo_bonus/$(NAME_BONUS)
	
re : fclean
	make all

.PHONY : all bonus clean fclean re san