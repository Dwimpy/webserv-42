# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: arobu <arobu@student.42heilbronn.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/21 14:38:01 by arobu             #+#    #+#              #
#    Updated: 2023/08/09 05:04:05 by arobu            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables
NAME			= webserv

# VPath
VPATH			= ./src ./src/Server ./src/ServerConfig ./src/SocketHandler ./src/HttpRequestHandler

# Includes
INCLUDE			= -I ./src/Server
INCLUDE			+= -I ./src/ServerConfig
INCLUDE			+= -I ./src/SocketHandler
INCLUDE			+= -I ./src/HttpRequestHandler

# Diretories
SRC_DIR			= ./src
OBJ_DIR			= ./obj

# Compiler
CC			= c++
CFLAGS		= -O3 -flto #-Wall -Werror -Wextra

ifdef FSANITIZE
	CFLAGS += -g3 -fsanitize=address
	LDLFLAGS += -g3 -fsanitize=address
endif

#Archive and Remove
RM			= rm -f
AR			= ar rcs

# Colors
DEF_COLOR = \033[0;39m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

# Sources
SRCS	= $(shell find ./src -name "*.cpp")

# Objects
OBJS	= $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

# Rules
all:
	@${MAKE} $(NAME) -j
$(NAME): $(OBJ_DIR) $(OBJS) $(MAIN_FILE)
	@$(CC) $(INCLUDE) $(FRAMEWORK) $(ASAN) $(OBJS) -o $@ -lm $(LDLFLAGS) $(LIBFLAGS)
	@echo "$(YELLOW)$(NAME)$(DEF_COLOR) $(CYAN)done.$(DEF_COLOR)"

$(OBJ_DIR)/%.o: %.cpp
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

show:
	@echo $(OBJS)

 clean:
			@$(RM) -rdf $(OBJ_DIR)
			@$(RM) -rdf $(DSYM)
			@echo "$(YELLOW)$(NAME)$(DEF_COLOR) $(CYAN)successfully cleaned!$(DEF_COLOR)"

fclean:		clean
			@$(RM) -f $(NAME)
			@echo "$(YELLOW)All$(DEF_COLOR) $(CYAN)objects successfully cleaned!$(DEF_COLOR)"

re:			fclean all

bonus:	libft	mlx	$(NAME)

.PHONY:		all clean fclean re