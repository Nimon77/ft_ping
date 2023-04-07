# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nsimon <nsimon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/29 17:07:11 by nsimon            #+#    #+#              #
#    Updated: 2023/04/07 15:27:04 by nsimon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ping

INC_PATH = ./include/

SRC_PATH = ./srcs/

SRC_NAME =	main.c \
			utils.c

CC = clang $(CFLAGS)

# CFLAGS = -Wall -Wextra -Werror -std=c99 -g
CFLAGS = -g

UNAME_S := $(shell uname -s)

SRC = $(addprefix $(SRC_PATH),$(SRC_NAME))

OBJ_PATH = ./objs/

OBJ_NAME = $(SRC_NAME:.c=.o)

OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))

LBPATH = ./libft/

LDFLAGS = -L$(LBPATH)

LDNAME = $(LBPATH)libft.a

LDINCL = -I$(LBPATH)includes

TEMP = .temp.a

COM_COLOR   = \033[0;34m
OBJ_COLOR   = \033[0;36m
OK_COLOR    = \033[0;32m
ERROR_COLOR = \033[0;31m
WARN_COLOR  = \033[0;33m
NO_COLOR    = \033[m

all: $(NAME)

$(NAME): $(OBJ)
	@make -sC $(LBPATH)
	@printf "$(COM_COLOR)Creation of $(NAME) ...$(NO_COLOR)"
	@$(CC) $(OBJ) $(LDNAME) -o$(NAME)
	@printf "$(OK_COLOR) $(NAME) created\n$(NO_COLOR)"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	@$(CC) -I$(INC_PATH) $(LDINCL) -o $@ -c $<

debug: CFLAGS += -DDEBUG
debug: re

clean:
	@make clean -sC $(LBPATH)
	@printf "$(WARN_COLOR)Removal of .o files of $(NAME) ...$(NO_COLOR)"
	@rm -f $(OBJ)
	@rmdir $(OBJ_PATH) 2> /dev/null || true
	@printf "$(ERROR_COLOR)Files .o deleted\n$(NO_COLOR)"

fclean: clean
	@make fclean -sC $(LBPATH)
	@printf "$(WARN_COLOR)Removal of $(NAME)...$(NO_COLOR)"
	@rm -f $(NAME) $(TEMP)
	@printf "$(ERROR_COLOR)Binary $(NAME) deleted\n$(NO_COLOR)"

re: fclean all

git:
	@git add .
	@git commit -m "$(NAME)"
	@git push

norme:
	norminette $(SRC)
	norminette $(INC_PATH)*.h

.PHONY: all, clean, fclean, re