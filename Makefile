NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = ./libft
LIBFT_PATH = $(LIBFT_DIR)/libft.a

SRC_DIR = ./src
PIPEX_DIR = $(SRC_DIR)/pipex
PARSE_DIR = ./parse
OBJ_DIR = ./obj

SRC_FILES = $(SRC_DIR)/main.c \
            $(SRC_DIR)/shell_loop.c \
            $(SRC_DIR)/env.c

PIPEX_FILES = $(PIPEX_DIR)/pipex_implementation.c \
              $(PIPEX_DIR)/pipes.c \
              $(PIPEX_DIR)/parse_paths.c \
              $(PIPEX_DIR)/parse_cmds.c \
              $(PIPEX_DIR)/init_files.c \
              $(PIPEX_DIR)/here_doc.c \
              $(PIPEX_DIR)/exec.c \
              $(PIPEX_DIR)/errors.c \
              $(PIPEX_DIR)/cleanup.c

PARSE_FILES = $(PARSE_DIR)/automats.c

SRC_OBJ = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
PIPEX_OBJ = $(PIPEX_FILES:$(PIPEX_DIR)/%.c=$(OBJ_DIR)/pipex_%.o)
PARSE_OBJ = $(PARSE_FILES:$(PARSE_DIR)/%.c=$(OBJ_DIR)/%.o)

OBJS = $(SRC_OBJ) $(PIPEX_OBJ) $(PARSE_OBJ)

all: $(LIBFT_PATH) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT_PATH) -lreadline

$(LIBFT_PATH):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/pipex_%.o: $(PIPEX_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(PARSE_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re