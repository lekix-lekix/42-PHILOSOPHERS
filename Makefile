# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/26 17:19:05 by kipouliq          #+#    #+#              #
#    Updated: 2024/04/09 15:43:38 by kipouliq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = philo

NAME_BONUS = philo_bonus

SRC = ./philosophers.c

OBJ = $(SRC:.c=.o)

BONUS_OBJS = $(SRCS_BONUS:.c=.o)

CC = cc

FLAGS = -Wall -Wextra -Werror -g3

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) -g $(OBJ) -o $(NAME)

bonus : $(BONUS_OBJS)
	make -C $(PATH_LIBFT)
	$(CC) $(FLAGS) $(BONUS_OBJS) $(LIBFT) -o $(NAME_BONUS) -g3 

%.o:%.c
	$(CC) $(FLAGS) -c $<  -o $@

clean :
	rm -f *o

san: $(OBJ)
	$(CC) $(FLAGS) -fsanitize=thread -g $(OBJ) -o $(NAME) 

fclean : clean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re : fclean
	make all

.PHONY : all bonus clean fclean re