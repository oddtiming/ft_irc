#--------------------------------#
#    Name and file information   #
#--------------------------------#
NAME			:= ircserv


CPP_FILES		:=	main.cpp \
					Channel.cpp \
					Client.cpp \
					Message.cpp \
					Server.cpp \
					commands/Nick.cpp \
					commands/User.cpp \
					commands/Quit.cpp \
					commands/Join.cpp \
					commands/Pass.cpp


INC_FILES		:=	defines.h \
					Channel.hpp \
					Client.hpp \
					Message.hpp \
					Server.hpp \
					replies.h \
					commands/Nick.hpp \
					commands/User.hpp \
					commands/Quit.hpp \
                    commands/Pass.hpp \
					commands/Join.hpp




#---------------------------------------------------------#
#   Directory information and object directory building   #
#---------------------------------------------------------#


INC_DIR			:= ./includes
INCS			= $(addprefix $(INC_DIR)/, $(INC_FILES))
INCS			:= 

SRC_DIR			= ./srcs
SRCS			= $(addprefix $(SRC_DIR)/, $(CPP_FILES))

OBJ_DIR			= ./obj
OBJS			= $(addprefix $(OBJ_DIR)/, $(CPP_FILES:.cpp=.o))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCS)
	@mkdir -p $(@D)
	@echo Compiling $@
	@$(CC) $(CFLAGS) -o $@ -c $<

#--------------------------------#
#  Compiler settings and flags   #
#--------------------------------#
CC				= c++
RM				= rm -rf
CFLAGS			= -Wall -Wextra -Werror -Wshadow -Wno-shadow -std=c++98 -I$(INC_DIR)

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