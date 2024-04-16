# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/26 17:19:05 by kipouliq          #+#    #+#              #
#    Updated: 2024/04/15 13:54:08 by kipouliq         ###   ########.fr        #
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
	./srcs/monitor_thread.c

OBJ = $(SRC:.c=.o)

BONUS_OBJS = $(SRCS_BONUS:.c=.o)

CC = cc

FLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) -g3 $(OBJ) -o $(NAME)

bonus : $(BONUS_OBJS)
	make -C $(PATH_LIBFT)
	$(CC) $(FLAGS) $(BONUS_OBJS) $(LIBFT) -o $(NAME_BONUS) -g3

%.o:%.c
	$(CC) $(FLAGS) -c $< -o $@ -g3

clean :
	rm -f ./srcs/*o

san: $(OBJ)
	$(CC) $(FLAGS) -fsanitize=thread -g3 $(OBJ) -o $(NAME) 

fclean : clean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re : fclean
	make all

.PHONY : all bonus clean fclean re