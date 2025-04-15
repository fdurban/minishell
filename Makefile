NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror


LIBFT_DIR = ./libft
LIBFT_PATH= $(LIBFT_DIR)/libft.a

SRC_DIR = ./src
SRC_FILES = $(SRC_DIR)/main.c \
						$(SRC_DIR)/shell_loop.c \
						$(SRC_DIR)/env/env.c \
						$(SRC_DIR)/env/env_utils.c \
						$(SRC_DIR)/builtins/pwd.c \
						$(SRC_DIR)/builtins/echo.c \
						$(SRC_DIR)/builtins/cd.c \
						$(SRC_DIR)/builtins/cd_utils.c \
						$(SRC_DIR)/builtins/builtin_dispatch.c \
						$(SRC_DIR)/builtins/export.c \
						$(SRC_DIR)/builtins/export_print.c \
						$(SRC_DIR)/builtins/unset.c \
						$(OBJ_DIR)/builtins/env_builtin.c \


OBJ_DIR = ./obj
OBJ = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_SRC = $(SRC_DIR)/test_builtins.c
TEST_OBJ = $(TEST_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/builtins/%.o)
TEST_DEPS = $(OBJ_DIR)/env/env.o \
						$(OBJ_DIR)/env/env_utils.o \
						$(OBJ_DIR)/builtins/echo.o \
						$(OBJ_DIR)/builtins/builtin_dispatch.o \
						$(OBJ_DIR)/builtins/cd.o \
						$(OBJ_DIR)/builtins/cd_utils.o \
						$(OBJ_DIR)/builtins/pwd.o \
						$(OBJ_DIR)/builtins/export.o \
						$(OBJ_DIR)/builtins/export_print.o \
						$(OBJ_DIR)/builtins/unset.o \
						$(OBJ_DIR)/builtins/env_builtin.o \


TEST_EXE = test_builtins

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
		mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
		@mkdir -p $(dir $@)
		$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_PATH) -lreadline -o $(NAME)

test_builtins: $(TEST_OBJ) $(TEST_DEPS)
				make -C $(LIBFT_DIR)
				$(CC) $(CFLAGS) $^ $(LIBFT_PATH) -o $(TEST_EXE)

norminette:
	norminette $(SRC_FILES) minishell.h

clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(OBJ)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)
	rm -f $(TEST_EXE)

re: fclean all

.PHONY: all clean fclean re