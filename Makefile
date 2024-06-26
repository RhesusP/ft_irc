# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/15 02:00:37 by cbernot           #+#    #+#              #
#    Updated: 2024/03/11 16:53:15 by cbernot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re

NAME	=	ircserv

# ------------------------------------------------------------------------------
# ALL FILES
# ------------------------------------------------------------------------------

# Files
LST_INC	=	ircserv.hpp Server.hpp User.hpp Channel.hpp Message.hpp Utils.hpp Commands.hpp Replies.hpp

LST_SRC	=	main.cpp \
			Server.cpp \
			User.cpp \
			Channel.cpp \
			Message.cpp Utils.cpp \

LST_CMD = Pass.cpp Commands.cpp Unknown.cpp Quit.cpp User.cpp Nick.cpp Motd.cpp Ping.cpp Join.cpp Part.cpp Mode.cpp Topic.cpp Kick.cpp Invite.cpp Privmsg.cpp Who.cpp Bot.cpp

# Directories
DIR_INC	=	inc/
DIR_SRC	=	src/
DIR_OBJ	=	.obj/
DIR_CMD = commands/


# Shortcuts
INCS			:=	$(addprefix $(DIR_INC), $(LST_INC))
SRCS			:=	$(addprefix $(DIR_SRC), $(LST_SRC)) \
					$(addprefix $(DIR_SRC)$(DIR_CMD), $(LST_CMD))
OBJS			:=	$(subst $(DIR_SRC), $(DIR_OBJ), $(SRCS:.cpp=.o))

# ------------------------------------------------------------------------------
# COMMAND VARIABLES
# ------------------------------------------------------------------------------

# Commands
CC				:=	c++
CFLAGS			:=	-Wall -Wextra -Werror -g -std=c++98
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
	@mkdir -p $(DIR_OBJ) \
	$(DIR_OBJ)$(DIR_CMD)
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
