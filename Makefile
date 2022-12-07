#--------------------------------#
#    Name and file information   #
#--------------------------------#
NAME			:= ircserv


CPP_FILES		:= main.cpp Server.cpp User.cpp

#---------------------------------------------------------#
#   Directory information and object directory building   #
#---------------------------------------------------------#
INC				:= ./includes

SRC_DIR			= ./srcs
SRCS			= $(addprefix $(SRC_DIR)/, $(CPP_FILES))

OBJ_DIR			= ./obj
OBJS			= $(addprefix $(OBJ_DIR)/, $(CPP_FILES:.cpp=.o))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@echo Compiling $@
	@$(CC) $(CFLAGS) -o $@ -c $<

#--------------------------------#
#  Compiler settings and flags   #
#--------------------------------#
CC				= c++
RM				= rm -rf
CFLAGS			= -Wall -Wextra -Werror -Wshadow -Wno-shadow -std=c++98 -I./includes

#--------------------------------#
#   Makefile rules and targets   #
#--------------------------------#

all:			$(NAME)
				@echo Done.

$(NAME):		$(OBJS)
				@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:			
				@$(RM) $(OBJ_DIR)
				@echo Clean complete.

fclean:			clean
				@$(RM) $(NAME)
				@echo Full clean complete.

re:				fclean $(NAME)

.PHONY:			all clean fclean re