.DEFAULT_GOAL := all
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
INC = include
NAME = WebServ
HEADER = $(INC)/Configure.hpp \
		 $(INC)/Response.hpp \
		 $(INC)/WebServ.hpp \
		 $(INC)/Location.hpp \
		 $(INC)/Server.hpp \
		 $(INC)/ft.hpp
SRC = src/Location.cpp	\
	   src/Server.cpp \
	   src/ft.cpp \
	   src/Response.cpp	\
	   src/WebServ.cpp \
	   src/main.cpp
OBJ = $(SRC:.cpp=.o)
DEP = $(SRC:.cpp=.d)
-include $(DEP)

.PHONY: all clean fclean re

all: $(NAME)


clean:
	rm -f $(OBJ)
	rm -f $(DEP)

fclean: clean
	rm -f $(NAME)

re: fclean
	$(MAKE) all

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^


%.o: %.cpp
	$(CC) $(CFLAGS) -MMD -MP -I$(INC) -c -o $@ $<
