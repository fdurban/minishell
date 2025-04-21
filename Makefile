NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror


LIBFT_DIR = ./libft
LIBFT_PATH= $(LIBFT_DIR)/libft.a

SRC_DIR = ./src
SRC_FILES = $(SRC_DIR)/main.c $(SRC_DIR)/shell_loop.c $(SRC_DIR)/env.c

PARSE_DIR = ./parse
PARSE_FILES = $(PARSE_DIR)/automats.c

TOKEN_DIR = ./tokenize
TOKEN_FILES = $(TOKEN_DIR)/tokenize.c

OBJ_DIR = ./obj
SRC_OBJ = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
PARSE_OBJ = $(PARSE_FILES:$(PARSE_DIR)/%.c=$(OBJ_DIR)/%.o)
TOKEN_OBJ = $(TOKEN_FILES:$(TOKEN_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(SRC_OBJ) $(PARSE_OBJ)
	make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(SRC_OBJ) $(PARSE_OBJ) $(TOKEN_OBJ) $(LIBFT_PATH) -lreadline -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(PARSE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TOKEN_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

norminette:
	norminette $(SRC_FILES) minishell.h

clean:
	make clean -C $(LIBFT_DIR)
	rm -rf $(OBJ_DIR)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re