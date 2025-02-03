#include "Server.hpp"

void	Server::verifyNick(int cfd, std::string newNick)
{
	std::regex	incorrect("^([#$:#&+%~,]\\S*|\\S*[,*.?!@ ]\\S*|\\+(q|a|o|h|v)\\S*)");

	if (std::regex_match(newNick, incorrect))
	{
		std::string msg = ":ft_irc 432 " + newNick + " :Erroneus nickname\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
	}
	else
	{
		if (isUniqueNick(newNick))
		{
			std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " NICK " + newNick + "\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
			_clients.at(getClientIndex(cfd)).setNickname(newNick);
		}
		else
		{
			std::string msg = ":ft_irc 433 " + _clients.at(getClientIndex(cfd)).getNick() + " " + newNick + " \r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
		}
	}
}

void	Server::nick(int cfd, std::string arg)
{
	std::vector<std::string>	params;

	std::cout << "Executing NICK" << std::endl;
	parser(arg, params);
	if (arg.find("WHOIS") == 0)
	{
		if (params.empty())
		{
			std::string msg = ":ft_irc 461 WHOIS :Not enough parameters\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
		}
		else if (params.at(0) == _clients.at(getClientIndex(cfd)).getUser())
		{
			std::string msg = ":ft_irc 320 " + _clients.at(getClientIndex(cfd)).getUser()  + " " + _clients.at(getClientIndex(cfd)).getNick() + " : Your nickname is " + _clients.at(getClientIndex(cfd)).getNick() + "\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
			msg = ":ft_irc 318 " + _clients.at(getClientIndex(cfd)).getUser()  + " " + _clients.at(getClientIndex(cfd)).getNick() + " :End of /WHOIS list\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
		}
		else
		{
			std::string msg = ":ft_irc 432 " + params.at(0) + " :Erroneus nickname\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
		}
	}
	else
	{
		if (params.empty() || params.at(0).empty())
		{
			std::string msg = ":ft_irc 431 " + _clients.at(getClientIndex(cfd)).getNick() + " :No nickname given\r\n";
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
}

//std::regex	wrongStart("^([#$:#&+%~,])");
		//std::regex	cantContain("[,*.?!@ ]");
		//std::regex	cantStart("^\\+(q|a|o|h|v)");
