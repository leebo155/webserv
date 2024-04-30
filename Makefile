CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
INC = -Iinclude
SERVER = server
CLIENT = client
SRV_SRCS = src/server.cpp
CLT_SRCS = src/client.cpp
SRV_OBJS = $(SRV_SRCS:.cpp=.o)
CLT_OBJS = $(CLT_SRCS:.cpp=.o)

.PHONY: all clean fclean re

all: $(SERVER) $(CLIENT)

clean:
	rm -f $(SRV_OBJS) $(CLT_OBJS)

fclean: clean
	rm -f $(SERVER) $(CLIENT)

re: fclean
	$(MAKE) all

$(SERVER): $(SRV_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT): $(CLT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^
