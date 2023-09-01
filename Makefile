# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asioud <asioud@42heilbronn.de>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/21 14:38:01 by arobu             #+#    #+#              #
#    Updated: 2023/08/31 20:53:22 by asioud           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables
NAME			= webserv

# VPath
VPATH			= ./src ./src/Server ./src/ServerConfig ./src/SocketHandler ./src/HttpRequestHandler \
				  ./src/HttpRequestHandler/Request ./src/HttpRequestHandler/Response ./src/HttpRequestHandler/Header \
				  ./src/ResourceManager ./src/Client ./src/ServerManager ./src/ConfigFile/ ./src/ConfigFile/Directive \
				  ./src/ConfigFile/LocationBlock ./src/ConfigFile/ServerBlock ./src/ConfigFile/ServerConfigurator \
				  ./src/Parser/HttpRequestParser ./src/Parser/ConfigFileParser ./src/Socket/ISocket ./src/Socket/ServerSocket \
				  ./src/Socket/ClientSocket


# Includes
INCLUDE			= -I ./src/Server
INCLUDE			+= -I ./src/ServerConfig
INCLUDE			+= -I ./src/SocketHandler
INCLUDE			+= -I ./src/HttpRequestHandler
INCLUDE			+= -I ./src/HttpRequestHandler/Request
INCLUDE			+= -I ./src/HttpRequestHandler/Response
INCLUDE			+= -I ./src/HttpRequestHandler/Header
INCLUDE			+= -I ./src/Page
INCLUDE			+= -I ./src/ResourceManager
INCLUDE			+= -I ./src/Client
INCLUDE			+= -I ./src/ServerManager
INCLUDE			+= -I ./src/ConfigFile

INCLUDE			+= -I ./src/ConfigFile/Directive
INCLUDE			+= -I ./src/ConfigFile/LocationBlock
INCLUDE			+= -I ./src/ConfigFile/ServerBlock
INCLUDE			+= -I ./src/ConfigFile/ServerConfigurator

INCLUDE			+= -I ./src/Parser/HttpRequestParser
INCLUDE			+= -I ./src/Parser/ConfigFileParser
INCLUDE			+= -I ./src/Socket/ISocket
INCLUDE			+= -I ./src/Socket/ServerSocket
INCLUDE			+= -I ./src/Socket/ClientSocket



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

rust:
	curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh

cgi:
	@cd ./src/cgi && cargo build --release

 clean:
			@$(RM) -rdf $(OBJ_DIR)
			@$(RM) -rdf $(DSYM)
			@echo "$(YELLOW)$(NAME)$(DEF_COLOR) $(CYAN)successfully cleaned!$(DEF_COLOR)"

fclean:		clean
			@$(RM) -f $(NAME)
			@echo "$(YELLOW)All$(DEF_COLOR) $(CYAN)objects successfully cleaned!$(DEF_COLOR)"

re:			fclean all

.PHONY:		all clean fclean re