#include "Server.hpp"

void	Server::verifyNick(int cfd, std::string newNick)
{
	std::regex	incorrect("^([#$:#&+%~,]\\S*|\\S*[,*.?!@ ]\\S*|\\+(q|a|o|h|v)\\S*)");

	if (std::regex_match(newNick, incorrect))
	{
		std::string msg = ":ircserv 432 " + newNick + " :Erroneus nickname\r\n";
		send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
	}
	else
	{
		if (isUniqueNick(newNick))
		{
			std::string msg = ":" + getClient(cfd).getNick() + "!" + getClient(cfd).getUser() + "@" + getClient(cfd).getIPa() + " NICK " + newNick + " \r\n";
			send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
			getClient(cfd).setNickname(newNick);
		}
		else
		{
			std::string msg = ":ircserv 433 " + getClient(cfd).getNick() + " " + newNick + " \r\n";
			send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
		}
	}
}

void	Server::nick(int cfd, std::string arg)
{
	std::vector<std::string>	params;

	parser(arg, params);
	if (params.empty() || params.at(0).empty())
	{
		std::string msg = ":ircserv 431 " + getClient(cfd).getUser() + " :No nickname given\r\n";
		send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
		return ;
	}
	else if (getClient(cfd).getNick().compare(params.at(0)) == 0)
	{}
	else
	{
		verifyNick(cfd, params.at(0));
	} 
}

//std::regex	wrongStart("^([#$:#&+%~,])");
		//std::regex	cantContain("[,*.?!@ ]");
		//std::regex	cantStart("^\\+(q|a|o|h|v)");
