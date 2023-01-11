# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nsimon <nsimon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/08/14 17:40:05 by nsimon            #+#    #+#              #
#    Updated: 2023/01/11 16:58:29 by nsimon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

# Path

OBJ_PATH = ./objs/

# Name

SRC_NAME    = 	ft_memset.c			\
		  		ft_bzero.c			\
		  		ft_memcpy.c			\
		  		ft_memccpy.c		\
		  		ft_memmove.c		\
		  		ft_memchr.c			\
		  		ft_memcmp.c			\
		  		ft_isalnum.c		\
		  		ft_isalpha.c		\
		  		ft_isascii.c		\
		  		ft_isdigit.c		\
		  		ft_isprint.c		\
		  		ft_strchr.c			\
		  		ft_strlcpy.c		\
		  		ft_strlen.c			\
				ft_strcmp.c			\
		  		ft_strncmp.c		\
		  		ft_strrchr.c		\
		  		ft_tolower.c		\
		  		ft_toupper.c		\
		  		ft_strlcat.c		\
		  		ft_strnstr.c		\
		  		ft_atoi.c			\
		  		ft_calloc.c			\
		  		ft_strdup.c			\
		  		ft_substr.c			\
		  		ft_strjoin.c		\
		  		ft_strtrim.c		\
		  		ft_split.c			\
		  		ft_itoa.c			\
		  		ft_strmapi.c		\
		  		ft_putchar_fd.c		\
				ft_putstr_fd.c		\
				ft_putendl_fd.c		\
				ft_putnbr_fd.c		\
				ft_lstnew.c			\
				ft_lstadd_front.c	\
				ft_lstsize.c		\
	    		ft_lstlast.c		\
	    		ft_lstadd_back.c	\
		  		ft_lstdelone.c		\
		  		ft_lstclear.c		\
		  		ft_lstiter.c		\
		  		ft_lstmap.c

OBJ_NAME = $(SRC_NAME:.c=.o)

# Files

OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))

# Flags

CC = gcc $(CFLAGS)

CFLAGS = -Wall -Wextra -Werror

COM_COLOR   = \033[0;34m
OBJ_COLOR   = \033[0;36m
OK_COLOR    = \033[0;32m
ERROR_COLOR = \033[0;31m
WARN_COLOR  = \033[0;33m
NO_COLOR    = \033[m

# Rules

all: $(NAME)

$(NAME): $(OBJ)
	@printf "$(COM_COLOR)Creation of $(NAME) ...$(NO_COLOR)"
	@ar rc $(NAME) $(OBJ)
	@ranlib $(NAME)
	@printf "$(OK_COLOR)$(NAME) created\n$(NO_COLOR)"

$(OBJ_PATH)%.o: %.c
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	@$(CC) -o $@ -c $<

clean:
	@printf "$(WARN_COLOR)Removal of .o files of $(NAME) ...$(NO_COLOR)"
	@rm -f $(OBJ)
	@rmdir $(OBJ_PATH) 2> /dev/null || true
	@printf "$(ERROR_COLOR)Files .o deleted\n$(NO_COLOR)"

fclean: clean
	@printf "$(WARN_COLOR)Removal of $(NAME)...$(NO_COLOR)"
	@rm -f $(NAME)
	@printf "$(ERROR_COLOR)Binary $(NAME) deleted\n$(NO_COLOR)"

re: fclean all

.PHONY: all, clean, fclean, re

norme:
	norminette $(SRC)
	norminette $(INC_PATH)*.h