NAME := ircserv

CC := c++

FLAGS := -Wall -Wextra -Werror

HEADERS := Server.hpp Client.hpp Channel.hpp

SOURCES := main.cpp\
			control/runServer.cpp\
			control/acceptNewClient.cpp\
			control/processClientInput.cpp\
			control/registerClient.cpp\
			control/runCommands.cpp\
			control/parser.cpp\
			join/join.cpp\
			join/joinUtils.cpp\
			privmsg/privmsg.cpp\
			privmsg/privmsgUtils.cpp\
			mode/mode.cpp\
			mode/modeUtils.cpp\
			mode/setOpRights.cpp\
			mode/setClientLimit.cpp\
			mode/setInviteStatus.cpp\
			mode/setTopicRestriction.cpp\
			mode/setKey.cpp\
			Server.cpp\
			Client.cpp\
			Channel.cpp\
			pong.cpp\
			nick.cpp\
			topic.cpp\
			kickSomeone.cpp\
			inviteRandos.cpp\
			quit.cpp

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