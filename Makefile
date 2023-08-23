
NAME            = Webserv
VPATH           = ./src


SRC_DIR         = ./src
OBJ_DIR         = ./obj

# Modules
# Compiler
CC          = c++ -Wall -Werror -Wextra
CFLAGS      =  #-march=nocona
ASAN        = #-fsanitize=address -g3
CFLAGS      =  #-Ofast -flto# -g3 -fsanitize=address -g3 #-g3 -Wall -Werror -Wextra -g3 #

#Archive and Remove
RM          = rm -f
AR          = ar rcs
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
#SRCS = src/mergeSort.cpp src/mergeSortD.cpp ./src/main.cpp
SRCS    = $(shell find ./src -name "*.cpp")
# Objects
OBJS    = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))
# Rules
all:
	@${MAKE} $(NAME) -j
$(NAME): $(OBJ_DIR) $(OBJS)
	@$(CC) $(INCLUDE) $(FRAMEWORK) $(ASAN) $(OBJS) -o $@ -lm $(LDLFLAGS) $(LIBFLAGS)
	@echo "$(YELLOW)Webserv$(DEF_COLOR) $(CYAN)done.$(DEF_COLOR)"

$(OBJ_DIR)/%.o: %.cpp
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
show:
	echo $(SRCS)
	echo $(OBJS)

 clean:
			@$(RM) -rdf $(OBJ_DIR)
			@$(RM) -rdf $(DSYM)
			@echo "$(YELLOW)Webserv$(DEF_COLOR) $(CYAN)successfully cleaned!$(DEF_COLOR)"

fclean:		clean
#			@make fclean -C $(LIBFT_FOLDER)
#			@rm -rdf $(GLFW_BUILD)
#			@rm -rdf $(MLX_BUILD)
			@$(RM) -f $(NAME)
			@echo "$(YELLOW)All$(DEF_COLOR) $(CYAN)objects successfully cleaned!$(DEF_COLOR)"

re:			fclean all

.PHONY:		all clean fclean re