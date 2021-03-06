# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zkerriga <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/09/10 17:29:48 by zkerriga          #+#    #+#              #
#    Updated: 2020/09/18 17:55:13 by matrus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

OBJ_DIR = bin
SRC_DIR = src
HDR_DIR = includes
LIB_DIR = libs
CLASS_DIR = classes

CC = gcc
INCLUDES =  -I./$(LIB_DIR)/libft -I./$(HDR_DIR) \
			-I./$(SRC_DIR)/$(CLASS_DIR)/environment -I./$(SRC_DIR)/$(CLASS_DIR)/word_work
FLAGS = -Wall -Wextra -Werror $(INCLUDES) #-O2

FILES = main.c \
		$(wildcard $(SRC_DIR)/*.c) \
		$(wildcard $(SRC_DIR)/read_line/*.c) \
		$(wildcard $(SRC_DIR)/parse_commands/*.c) \
		$(wildcard $(SRC_DIR)/exec_all_commands/*.c) \
		$(wildcard $(SRC_DIR)/builtin_functions/*.c) \
		$(wildcard $(SRC_DIR)/signal_handlers/*.c) \
		$(wildcard $(SRC_DIR)/$(CLASS_DIR)/environment/*.c) \
		$(wildcard $(SRC_DIR)/$(CLASS_DIR)/word_work/*.c) \
		$(wildcard $(SRC_DIR)/errors/*.c)

FILES_O = $(addprefix $(OBJ_DIR)/, $(FILES:.c=.o))

.PHONY: all
all: $(OBJ_DIR) lft $(NAME)
	@echo "\n\033[32m[+] The $(NAME) assembled!\033[0m\n"

$(OBJ_DIR):
	@mkdir -p	$(OBJ_DIR)/$(SRC_DIR)/read_line/ \
				$(OBJ_DIR)/$(SRC_DIR)/parse_commands/ \
				$(OBJ_DIR)/$(SRC_DIR)/exec_all_commands/ \
				$(OBJ_DIR)/$(SRC_DIR)/builtin_functions/ \
				$(OBJ_DIR)/$(SRC_DIR)/signal_handlers/ \
				$(OBJ_DIR)/$(SRC_DIR)/errors/ \
				$(OBJ_DIR)/$(SRC_DIR)/$(CLASS_DIR)/environment \
				$(OBJ_DIR)/$(SRC_DIR)/$(CLASS_DIR)/word_work

.PHONY: lft
lft:
	@$(MAKE) -C ./$(LIB_DIR)/libft --no-print-directory --silent
	@echo "\033[32m[+] Libft is assembled!\033[0m"

$(NAME): $(FILES_O)
	$(CC) $(FLAGS) $(FILES_O) -L./$(LIB_DIR)/libft/ -lft -o $(NAME)

$(FILES_O): $(OBJ_DIR)/%.o: %.c
	@$(CC) $(FLAGS) -MMD -c $< -o $@
include $(wildcard $(OBJ_DIR)/*.d $(OBJ_DIR)/*/*.d $(OBJ_DIR)/*/*/*.d $(OBJ_DIR)/*/*/*/*.d)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: re
re: fclean libfclean all

.PHONY: bonus
bonus: all

.PHONY: libfclean
libfclean:
	@$(MAKE) -C ./$(LIB_DIR)/libft --no-print-directory fclean

test:
	gcc test.c $(INCLUDES) -L./$(LIB_DIR)/libft -lft -o test_program && ./test_program
