#include "Server.hpp"

Server::Command	Server::identifyCommand(std::string *cmd) {

	enum	Command command = FAIL;
	std::size_t		found = 0;

	if ((*cmd).find("KICK") != std::string::npos)
	{
		command = KICK;
		found = (*cmd).find("KICK");
	}
	else if ((*cmd).find("INVITE") != std::string::npos)
	{
		command = INVITE;
		found = (*cmd).find("INVITE");
	}
	else if ((*cmd).find("TOPIC") != std::string::npos)
	{
		command = TOPIC;
		found = (*cmd).find("TOPIC");
	}
	else if ((*cmd).find("MODE") != std::string::npos)
	{
		command = MODE;
		found = (*cmd).find("MODE");
	}
	else if ((*cmd).find("JOIN") != std::string::npos)
	{
		command = JOIN;
		found = (*cmd).find("JOIN");
	}
	else if ((*cmd).find("PRIVMSG") != std::string::npos)
	{
		command = PRIVMSG;
		found = (*cmd).find("PRIVMSG");
	}
	else if ((*cmd).find("NICK") != std::string::npos)
	{
		command = NICK;
		found = (*cmd).find("NICK");
	}
	else if ((*cmd).find("PING") != std::string::npos)
	{
		command = PING;
		found = (*cmd).find("PING");
	}
	else if ((*cmd).find("QUIT") != std::string::npos)
	{
		command = QUIT;
		found = (*cmd).find("QUIT");
	}
	/* else if ((*cmd).find("WHOIS") != std::string::npos)
	{
		command = WHOIS;
		found = (*cmd).find("WHOIS");
	} */
	if (found != std::string::npos && found != 0)
		*cmd = (*cmd).substr(found);
	return (command);
}

void	Server::runCommand(int cfd, std::string arg, size_t *clientIndex)
{
	enum	Command cmd = identifyCommand(&arg);
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
		case 9:
			//return (whois(cfd, arg));
		default :
			std::cout << "Unknown command" << std::endl;
			// unknown command, also errors will go here....?
	}
}