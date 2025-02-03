#include "Server.hpp"

Server::Command	Server::identifyCommand(std::string cmd) {

	if (cmd.compare("KICK") == 0 || cmd.compare(0, 5, "KICK ") == 0)
		return (KICK);
	if (cmd.compare("INVITE") == 0 || cmd.compare(0, 7, "INVITE ") == 0)
		return (INVITE);
	if (cmd.compare("TOPIC") == 0 || cmd.compare(0, 6, "TOPIC ") == 0)
		return (TOPIC);
	if (cmd.compare("MODE") == 0 || cmd.compare(0, 5, "MODE ") == 0)
		return (MODE);
	if (cmd.compare("JOIN") == 0 || cmd.compare(0, 5, "JOIN ") == 0)
		return (JOIN);
	if (cmd.compare("PRIVMSG") == 0 || cmd.compare(0, 8, "PRIVMSG ") == 0)
		return (PRIVMSG);
	if (cmd.compare("NICK") == 0 || cmd.compare(0, 5, "NICK ") == 0 || cmd.compare("WHOIS") == 0 || cmd.compare(0, 6, "WHOIS ") == 0)
		return (NICK);
	if (cmd.compare("PING") == 0 || cmd.compare(0, 5, "PING ") == 0)
		return (PING);
	if (cmd.compare("QUIT") == 0 || cmd.compare(0, 5, "QUIT ") == 0)
		return (QUIT);
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
			//setTopic();
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
			std::cout << "Unknown command" << std::endl;
			// unknown command, also errors will go here....?
	}
}