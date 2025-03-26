NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror


LIBFT_DIR = ./libft
LIBFT_PATH= $(LIBFT_DIR)/libft.a

SRC_DIR = ./src
SRC_FILES = $(SRC_DIR)/replaceline_test.c

OBJ_DIR = ./obj
OBJ = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
	make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_PATH) -lreadline -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS)  -c $< -o $@

norminette:
	norminette $(SRC_FILES) minishell.h

clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(OBJ)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(OBJ)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re