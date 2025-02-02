#include "Server.hpp"

void	Server::verifyNick(int cfd, std::string newNick)
{
	std::regex	incorrect("^([#$:#&+%~,]\\S*|\\S*[,*.?!@ ]\\S*|\\+(q|a|o|h|v)\\S*)");

	if (std::regex_match(newNick, incorrect))
	{
		std::string msg = ":ircserv 432 " + newNick + " :Erroneus nickname\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
	}
	else
	{
		if (isUniqueNick(newNick))
		{
			std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " NICK " + newNick + " \r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
			_clients.at(getClientIndex(cfd)).setNickname(newNick);
		}
		else
		{
			std::string msg = ":ircserv 433 " + _clients.at(getClientIndex(cfd)).getNick() + " " + newNick + " \r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
		}
	}
}

void	Server::nick(int cfd, std::string arg)
{
	std::vector<std::string>	params;

	parser(arg, params);
	if (params.empty() || params.at(0).empty())
	{
		std::string msg = ":ircserv 431 " + _clients.at(getClientIndex(cfd)).getUser() + " :No nickname given\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
		return ;
	}
	else if (_clients.at(getClientIndex(cfd)).getNick().compare(params.at(0)) == 0)
	{}
	else
	{
		verifyNick(cfd, params.at(0));
	} 
}

//std::regex	wrongStart("^([#$:#&+%~,])");
		//std::regex	cantContain("[,*.?!@ ]");
		//std::regex	cantStart("^\\+(q|a|o|h|v)");
