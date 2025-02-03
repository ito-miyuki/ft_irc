#include "Server.hpp"

Server::Command	Server::identifyCommand(std::string command)
{
	size_t	divider = command.find(' ');
	if (divider != std::string::npos)
	{
		std::string cmd = command.substr(0, divider);

		if (cmd.compare("KICK") == 0)
			return (KICK);
		if (cmd.compare("INVITE") == 0)
			return (INVITE);
		if (cmd.compare("TOPIC") == 0)
			return (TOPIC);
		if (cmd.compare("MODE") == 0)
			return (MODE);
		if (cmd.compare("JOIN") == 0)
			return (JOIN);
		if (cmd.compare("PRIVMSG") == 0)
			return (PRIVMSG);
		if (cmd.compare("NICK") == 0)
			return (NICK);
		if (cmd.compare("PING") == 0)
			return (PING);
		if (cmd.compare("QUIT") == 0)
			return (QUIT);
	}
	return (FAIL);
}

void	Server::runCommand(int cfd, std::string arg, size_t *clientIndex)
{
	enum	Command cmd = identifyCommand(arg);
	int		cmdID = cmd;

	switch (cmdID)
	{
		case 0:
			return kickSomeone(cfd, arg);
		case 1:
			return inviteRandos(cfd, arg);
		case 2:
			return (topic(cfd, arg));
		case 3:
			return (mode(cfd, arg));
		case 4:
			return (join(cfd, arg));
		case 5:
			return (messages(cfd, arg));
		case 6:
			return (nick(cfd, arg));
		case 7:
			return (pingMyPong(cfd, arg));
		case 8:
			return (quit(cfd, arg, clientIndex));
		default :
			std::cout << "It doesn't let me compile without this" << std::endl;
			// unknown command, also errors will go here....?
	}
}