NAME := ircserv

CC := c++

FLAGS := -Wall -Wextra -Werror

HEADERS := Server.hpp Client.hpp Channel.hpp

SOURCES := main.cpp\
			runServer.cpp\
			Server.cpp\
			Client.cpp\
			acceptNewClient.cpp\
			processClientInput.cpp

OBJECTS := $(SOURCES:.cpp=.o)

all: $(NAME)

%.o:%.cpp
	$(CC) $(FLAGS) -c $< -o $(<:.cpp=.o)

$(NAME): $(OBJECTS) $(HEADERS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(NAME)

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY = all fclean re clean