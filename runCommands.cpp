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

void	Server::runCommand(int cfd, std::string arg)
{
	enum	Command cmd = identifyCommand(arg);
	int		cmdID = cmd;

	switch (cmdID)
	{
		case 0:
			//kickSomeone();
		case 1:
			//inviteRandos();
		case 2:
			//setTopic();
		case 3:
			//changeMyModesBaby();
		case 4:
			//forGroupies() join;
		case 5:
			//slideIntoMyDMs();
		case 6:
			return (updateNick(cfd, arg));
		case 7:
			//pingMyPong();
		case 8:
			//imSoDONE();
		default:
			// unknown command, also errors will go here....?
	}
}