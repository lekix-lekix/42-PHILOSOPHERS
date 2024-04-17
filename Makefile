# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/26 17:19:05 by kipouliq          #+#    #+#              #
#    Updated: 2024/04/17 15:57:03 by kipouliq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = philo

NAME_BONUS = philo_bonus

SRC = ./srcs/main.c \
	./srcs/routines.c \
	./srcs/routines_bis.c \
	./srcs/init_data.c \
	./srcs/checks.c \
	./srcs/free_destroy.c \
	./srcs/utils.c \
	./srcs/monitor_thread.c \
	./srcs/ft_itoa.c \
	./srcs/args_checking.c \
	./srcs/utils_2.c

SRC_BONUS = ./philo_bonus/srcs/main.c \
	./philo_bonus/srcs/ft_itoa.c \
	./philo_bonus/srcs/args_checking.c \
	./philo_bonus/srcs/utils.c \
	./philo_bonus/srcs/utils_2.c

OBJ = $(SRC:.c=.o)

BONUS_OBJS = $(SRC_BONUS:.c=.o)

CC = cc

FLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) -g3 $(OBJ) -o $(NAME)

bonus : $(NAME_BONUS)

$(NAME_BONUS) : $(BONUS_OBJS)
	$(CC) $(FLAGS) -g3 $(BONUS_OBJS) -o $(NAME_BONUS)

%.o:%.c
	$(CC) $(FLAGS) -c $< -o $@ -g3

clean :
	rm -f ./srcs/*o
	rm -f ./philo_bonus/srcs/*o

san: $(OBJ)
	$(CC) $(FLAGS) -fsanitize=thread -g3 $(OBJ) -o $(NAME) 

fclean : clean
	rm -f $(NAME)
	rm -f ./philo_bonus/$(NAME_BONUS)
	
re : fclean
	make all

.PHONY : all bonus clean fclean re san