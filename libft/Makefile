NAME = libft.a
CC = gcc

STANDARD_DIR = standard
PRINTF_DIR = printf
GNL_DIR = get_next_line

CFILES = $(STANDARD_DIR)/ft_isalpha.c $(STANDARD_DIR)/ft_isdigit.c $(STANDARD_DIR)/ft_isalnum.c $(STANDARD_DIR)/ft_isascii.c \
				 $(STANDARD_DIR)/ft_isprint.c $(STANDARD_DIR)/ft_strlen.c $(STANDARD_DIR)/ft_memset.c $(STANDARD_DIR)/ft_bzero.c \
         $(STANDARD_DIR)/ft_memcpy.c $(STANDARD_DIR)/ft_memmove.c $(STANDARD_DIR)/ft_strlcpy.c $(STANDARD_DIR)/ft_strlcat.c \
         $(STANDARD_DIR)/ft_toupper.c $(STANDARD_DIR)/ft_tolower.c $(STANDARD_DIR)/ft_strchr.c $(STANDARD_DIR)/ft_strrchr.c \
         $(STANDARD_DIR)/ft_strncmp.c $(STANDARD_DIR)/ft_memchr.c $(STANDARD_DIR)/ft_memcmp.c $(STANDARD_DIR)/ft_strnstr.c \
         $(STANDARD_DIR)/ft_atoi.c $(STANDARD_DIR)/ft_calloc.c $(STANDARD_DIR)/ft_strdup.c $(STANDARD_DIR)/ft_substr.c \
         $(STANDARD_DIR)/ft_strjoin.c $(STANDARD_DIR)/ft_strtrim.c $(STANDARD_DIR)/ft_split.c $(STANDARD_DIR)/ft_itoa.c \
         $(STANDARD_DIR)/ft_strmapi.c $(STANDARD_DIR)/ft_striteri.c $(STANDARD_DIR)/ft_putchar_fd.c $(STANDARD_DIR)/ft_putstr_fd.c \
         $(STANDARD_DIR)/ft_putendl_fd.c $(STANDARD_DIR)/ft_putnbr_fd.c

BONUSFILES = $(STANDARD_DIR)/ft_lstnew_bonus.c $(STANDARD_DIR)/ft_lstadd_front_bonus.c $(STANDARD_DIR)/ft_lstsize_bonus.c \
             $(STANDARD_DIR)/ft_lstlast_bonus.c $(STANDARD_DIR)/ft_lstadd_back_bonus.c $(STANDARD_DIR)/ft_lstdelone_bonus.c \
             $(STANDARD_DIR)/ft_lstclear_bonus.c $(STANDARD_DIR)/ft_lstiter_bonus.c $(STANDARD_DIR)/ft_lstmap_bonus.c

PRINTF_FILES = $(PRINTF_DIR)/ft_printf_utils.c $(PRINTF_DIR)/ft_printf.c
GNL_FILES = $(GNL_DIR)/get_next_line.c $(GNL_DIR)/get_next_line_utils.c

# Archivos objeto
OFILES = $(CFILES:.c=.o)
PRINTF_OFILES = $(PRINTF_FILES:.c=.o)
GNL_OFILES = $(GNL_FILES:.c=.o)
BONUS_OFILES = $(BONUSFILES:.c=.o)

CFLAGS = -Wall -Wextra -Werror
REMOVE = rm -rf

all: $(NAME)

$(NAME): $(OFILES) $(PRINTF_OFILES) $(GNL_OFILES) $(BONUS_OFILES)
	ar rcs $(NAME) $(OFILES) $(PRINTF_OFILES) $(GNL_OFILES) $(BONUS_OFILES)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(REMOVE) $(OFILES) $(PRINTF_OFILES) $(BONUS_OFILES) $(GNL_OFILES)

fclean: clean
	$(REMOVE) $(NAME)

re: fclean all

.PHONY: fclean clean re all
