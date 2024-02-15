# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: svanmeen <svanmeen@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/15 02:00:37 by cbernot           #+#    #+#              #
#    Updated: 2024/02/08 12:26:06 by svanmeen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re

NAME	=	ircserv

# ------------------------------------------------------------------------------
# ALL FILES
# ------------------------------------------------------------------------------

# Files
LST_INC	=	ircserv.hpp Server.hpp User.hpp Channel.hpp Exceptions.hpp
LST_SRC	=	main.cpp utils.cpp \
			Server.cpp Server_init.cpp Server_utils.cpp \
			User.cpp \
			Channel.cpp \
			Exceptions.cpp

# Directories
DIR_INC	=	inc/
DIR_SRC	=	src/
DIR_OBJ	=	.obj/

# Shortcuts
INCS			:=	$(addprefix $(DIR_INC), $(LST_INC))
SRCS			:=	$(addprefix $(DIR_SRC), $(LST_SRC))
OBJS			:=	$(subst $(DIR_SRC), $(DIR_OBJ), $(SRCS:.cpp=.o))

# ------------------------------------------------------------------------------
# COMMAND VARIABLES
# ------------------------------------------------------------------------------

# Commands
CC				:=	c++
CFLAGS			:=	-g #-Wall -Wextra -Werror -std=c++98
CI				:=	-I $(DIR_INC)
RM				:=	rm -rf

# Colors
BLACK=\033[30m
RED=\033[31m
GREEN=\033[32m
YELLOW=\033[33m
BLUE=\033[34m
PURPLE=\033[35m
CYAN=\033[36m
WHITE=\033[37m

# Text
ERASE=\033[2K\r
RESET=\033[0m
BOLD=\033[1m
FAINT=\033[2m
ITALIC=\033[3m
UNDERLINE=\033[4m

# ------------------------------------------------------------------------------
# RULES
# ------------------------------------------------------------------------------

all: $(NAME)

$(NAME): $(OBJS) $(INCS)
	@$(CC) $(CFLAGS) $(CI) $(OBJS) -o $@
	@echo "$(ERASE)\n$(PURPLE)$(BOLD)🎉🎉 executable $(NAME) created 🎉🎉$(RESET)"

$(DIR_OBJ)%.o: $(DIR_SRC)%.cpp $(INCS) | $(DIR_OBJ)
	@$(CC) $(CFLAGS) $(CI) -c $< -o $@
	@echo "$(ERASE)$(BOLD)$(NAME)$(RESET)$(GREEN) compiling... $(RESET)$<"

$(DIR_OBJ):
	@mkdir -p $(DIR_OBJ)
	@echo "$(ERASE)$(BOLD)$(NAME)$(RESET)$(GREEN) objects directory created.$(RESET)"

# Cleaning
clean:
	@$(RM) $(OBJS)
	@if [ -d "$(DIR_OBJ)" ]; then \
		$(RM) $(DIR_OBJ); echo "$(ERASE)$(BOLD)$(NAME)$(RESET)$(RED) objects deleted.$(RESET)"; \
	fi

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		$(RM) $(NAME); echo "$(ERASE)$(RED)$(BOLD)🗑️  $(NAME) deleted.$(RESET)"; \
	fi

re: fclean all
