# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: quackson <quackson@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/21 16:08:07 by abaiao-r          #+#    #+#              #
#    Updated: 2023/04/24 01:03:13 by quackson         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler settings
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -lreadline# -fsanitize=address 

# Directories
SRCDIR = ./src
OBJDIR = ./objs

# Source Files

SRCS = $(SRCDIR)/main.c $(SRCDIR)/utils_1.c $(SRCDIR)/utils_2.c $(SRCDIR)/command_utils.c \
	   $(SRCDIR)/commands.c $(SRCDIR)/print_prompt.c $(SRCDIR)/env.c
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBFT_DIR = libft/
LIBFT_INCLUDE = libft

# Targets
NAME = minishell

all: 	$(NAME)

bonus:	all

clean:
		rm -rf $(OBJDIR)
		cd libft && make clean

fclean:	clean
		rm -f $(NAME)
		cd libft && make fclean

run:	all
		./$(NAME)

re:		fclean all

lldb:	all
		lldb -- ./$(NAME)

gdb:	all
		gdb --args $(NAME)

valgrind: 	all
			valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

$(NAME): 	$(OBJS)
			$(MAKE) -C $(LIBFT_DIR) bonus
			$(CC) $(OBJS) $(CFLAGS) libft/libft.a -o $(NAME)

%.o: %.c
	$(CC) -Wall -Wextra -Werror -O3 -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean bonus run