#include "Server.hpp"

void	Server::nick(int cfd, std::string arg)
{
	std::stringstream	ss(arg);
	std::string			substr;
	char				del = ' ';
	int					argCount = 0;
	std::string			newNick = "";

	while (getline(ss, substr, del))
	{
		if (argCount == 1)
		{
			newNick = substr;
			break ;
		}
		argCount++;
	}
	if (newNick.empty())
	{
		std::string msg = ":ft_irc 431 " + getClient(cfd).getUser() + " :No nickname given\r\n";
		send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
		return ;
	}
	else if (getClient(cfd).getNick().compare(newNick) == 0)
	{}
	else
	{
		//std::regex	wrongStart("^([#$:#&+%~,])");
		//std::regex	cantContain("[,*.?!@ ]");
		//std::regex	cantStart("^\\+(q|a|o|h|v)");
		std::regex	incorrect("^([#$:#&+%~,]\\S*|\\S*[,*.?!@ ]\\S*|\\+(q|a|o|h|v)\\S*)");

		if (std::regex_match(newNick, incorrect))
		{
			std::string msg = ":ft_irc 432 " + newNick + " :Erroneus nickname\r\n";
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
				//std::string msg = ":ft_irc 433 " + newNick + " \r\n";
				std::string msg = ":ft_irc 433 \r\n";
				//std::string msg = ":ft_irc 433 " + getClient(cfd).getNick() + " " + newNick + " \r\n";
				send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
			}
		}
	} 
}
