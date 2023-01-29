#--------------------------------#
#    Name and file information   #
#--------------------------------#
NAME			:= ircBot

CPP_FILES		:=	main.cpp \
					Bot.cpp \
					TimeBot.cpp \
					JokeBot.cpp \
					DictBot.cpp



INC_FILES		:=	Bot.hpp \
					TimeBot.hpp \
					JokeBot.hpp \
					DictBot.hpp


#---------------------------------------------#
#    Information for run arguments and PIDs   #
#---------------------------------------------#

ARG1=time
ARG2=joke
ARG3=dict
PID1=timeBot.pid
PID2=jokeBot.pid
PID3=dictBot.pid

#---------------------------------------------------------#
#   Directory information and object directory building   #
#---------------------------------------------------------#


INC_DIR			:= ./includes
INCS			= $(addprefix $(INC_DIR)/, $(INC_FILES))

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
CFLAGS			= -Wall -Wextra -Werror -Wshadow -Wno-shadow -std=c++98 -I$(INC_DIR) -g
LINKFLAGS		= -lcurl

#--------------------------------#
#   Makefile rules and targets   #
#--------------------------------#

all:			$(NAME)
				@echo Compiled executable $(NAME)



run: all
	@nohup ./$(NAME) $(ARG1) > /dev/null 2>&1 & echo $$! > $(PID1)
	@nohup ./$(NAME) $(ARG2) > /dev/null 2>&1 & echo $$! > $(PID2)
	@nohup ./$(NAME) $(ARG3) > /dev/null 2>&1 & echo $$! > $(PID3)
	@echo "Launched $(ARG1)Bot, $(ARG2)Bot, and $(ARG3)Bot"

clean:
	@if [ -f $(PID1) ]; then \
	  kill `cat $(PID1)` 2&> /dev/null; \
	  echo "Killed program instance with PID `cat $(PID1)`"; \
	fi
	@if [ -f $(PID2) ]; then \
	  kill `cat $(PID2)` 2&> /dev/null; \
	  echo "Killed program instance with PID `cat $(PID2)`"; \
	fi
	@if [ -f $(PID3) ]; then \
	  kill `cat $(PID3)` 2&> /dev/null; \
	  echo "Killed program instance with PID `cat $(PID3)`"; \
	fi
	@rm -f $(PID1) $(PID2) $(PID3)
	@$(RM) $(OBJ_DIR)
	@echo "Bots Clean complete."



$(NAME):		$(OBJS)
				@$(CC) $(CFLAGS) $(LINKFLAGS) -o $(NAME) $(OBJS)


fclean:			clean
				@$(RM) $(NAME)
				@echo Bots Full clean complete.

re:				fclean $(NAME)

.PHONY:			all clean fclean re