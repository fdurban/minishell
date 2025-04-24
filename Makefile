NAME = minishell
TEST_EXE = test_execution

CC = gcc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = ./libft
LIBFT_PATH = $(LIBFT_DIR)/libft.a

SRC_DIR = ./src
EXE_FILES = $(SRC_DIR)/execution/execution.c $(SRC_DIR)/execution/cleanup.c $(SRC_DIR)/execution/errors.c $(SRC_DIR)/execution/exec.c
PARSE_DIR = $(SRC_DIR)/parse
ENV_FILE = $(SRC_DIR)/env.c
ENV_OBJ = $(ENV_FILE:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_DIR = ./obj

# Main shell source files
SRC_FILES = $(SRC_DIR)/main.c \
            $(SRC_DIR)/shell_loop.c \
            $(SRC_DIR)/env.c

# Parse files (used by execution code)
PARSE_FILES = $(PARSE_DIR)/automats.c

SRC_OBJ   = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXE_OBJ   = $(EXE_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
PARSE_OBJ = $(PARSE_FILES:$(PARSE_DIR)/%.c=$(OBJ_DIR)/parse/%.o)

OBJS = $(SRC_OBJ) $(PARSE_OBJ)

TEST_OBJS = $(SRC_OBJ)

# New test source for execution (bypassing parsing/tokenization)
TEST_SRC = $(SRC_DIR)/execution/test_execution.c
TEST_OBJ = $(TEST_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(LIBFT_PATH) $(NAME)

$(NAME): $(OBJS) $(EXE_OBJ)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(EXE_OBJ) $(LIBFT_PATH) -lreadline

test_execution: $(TEST_OBJ) $(EXE_OBJ) $(ENV_OBJ) $(LIBFT_PATH)
		$(CC) $(CFLAGS) -o $(TEST_EXE) $(TEST_OBJ) $(EXE_OBJ) $(ENV_OBJ) $(LIBFT_PATH) -lreadline

$(LIBFT_PATH):
		$(MAKE) -C $(LIBFT_DIR)

# Generic pattern rule for files in src/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
		@mkdir -p $(dir $@)
		$(CC) $(CFLAGS) -c $< -o $@

# Specific rule for parse files
$(OBJ_DIR)/parse/%.o: $(PARSE_DIR)/%.c
		@mkdir -p $(dir $@)
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -rf $(OBJ_DIR)
		$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
		rm -f $(NAME)
		rm -f $(TEST_EXE)
		$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re test_execution