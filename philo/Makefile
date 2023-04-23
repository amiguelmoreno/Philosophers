# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: antmoren <antmoren@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/16 12:19:44 by antmoren          #+#    #+#              #
#    Updated: 2023/04/23 21:35:44 by antmoren         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	philosophers
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror

SRC_PATH 		= 	./src/
OBJ_PATH 		= 	./obj/

SRCS	= 	main.c				\
			utils.c 			\
			input_checker.c 	\
			init.c 				\
			actions.c 			\
			doctor.c			\
			time.c				\
			display.c 			\
			

OBJS 	= $(addprefix $(OBJ_PATH), $(SRCS:.c=.o))


all: $(OBJ_PATH) $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@$(CC) $(CFLAGS) $(INCS) $(HEADERS) -c $< -o $@

$(OBJ_PATH): 
	@mkdir $(OBJ_PATH)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)
	@echo "\n 👾 $(NAME) compiled! ✅"

clean:
	@rm -rf $(OBJ_PATH)
	@echo "\n  👾 $(NAME) executable files removed! 🗑"

fclean: clean
	@rm -f $(NAME)
	@echo "\n  👾 $(NAME) generated files removed! 🗑"

re: clean all
	@echo "\n 🔄  ✅ Cleaned and rebuilt everything from Fdf"

.PHONY: all, clean, fclean, re