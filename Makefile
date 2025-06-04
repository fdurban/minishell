NAME = minishell
CC = cc
CFLAGS = -g -O0 -Wall -Wextra -Werror


LIBFT_DIR = ./libft
LIBFT_PATH= $(LIBFT_DIR)/libft.a

SRC_DIR = ./src
SRC_FILES = $(SRC_DIR)/main.c

ENV_DIR  = $(SRC_DIR)/env
ENV_FILES = $(ENV_DIR)/env.c $(ENV_DIR)/env_utils.c 

LOOP_DIR  = $(SRC_DIR)/shell_loop
LOOP_FILES = $(LOOP_DIR)/shell_loop.c $(ENV_DIR)/shell_loop_utils.c 

PARSE_DIR = $(SRC_DIR)/parse
PARSE_FILES = $(PARSE_DIR)/automats.c

TOKEN_DIR = $(SRC_DIR)/tokenize
TOKEN_FILES = $(TOKEN_DIR)/tokenize.c $(TOKEN_DIR)/tokenize_utils.c \
			  $(TOKEN_DIR)/token_extraction.c $(TOKEN_DIR)/variable_expansion.c \
			  $(TOKEN_DIR)/debug.c $(TOKEN_DIR)/token_node.c $(TOKEN_DIR)/token_handlers.c

BUILTINS_DIR = $(SRC_DIR)/builtins
BUILTINS_FILES = $(BUILTINS_DIR)/unset.c $(BUILTINS_DIR)/pwd.c $(BUILTINS_DIR)/export_print.c \
                 $(BUILTINS_DIR)/export.c $(BUILTINS_DIR)/exit.c $(BUILTINS_DIR)/env_builtin.c \
                 $(BUILTINS_DIR)/echo.c $(BUILTINS_DIR)/cd_utils.c $(BUILTINS_DIR)/cd.c \
                 $(BUILTINS_DIR)/builtin_dispatch.c

OBJ_DIR = ./obj
SRC_OBJ = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
ENV_OBJ = $(ENV_FILES:$(ENV_DIR)/%.c=$(OBJ_DIR)/%.o)
LOOP_OBJ = $(LOOP_FILES:$(ENV_DIR)/%.c=$(OBJ_DIR)/%.o)
PARSE_OBJ = $(PARSE_FILES:$(PARSE_DIR)/%.c=$(OBJ_DIR)/%.o)
TOKEN_OBJ = $(TOKEN_FILES:$(TOKEN_DIR)/%.c=$(OBJ_DIR)/%.o)
BUILTINS_OBJ = $(BUILTINS_FILES:$(BUILTINS_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(SRC_OBJ) $(ENV_OBJ) $(LOOP_OBJ) $(PARSE_OBJ) $(TOKEN_OBJ) $(BUILTINS_OBJ)
	make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(SRC_OBJ) $(LOOP_OBJ) $(PARSE_OBJ) $(TOKEN_OBJ) $(BUILTINS_OBJ) $(ENV_OBJ) $(LIBFT_PATH) -lreadline -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(ENV_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(LOOP_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(PARSE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TOKEN_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(BUILTINS_DIR)/%.c
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