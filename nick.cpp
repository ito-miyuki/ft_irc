#include "Server.hpp"

void	Server::verifyNick(int cfd, std::string newNick) {

	std::regex	incorrect("^([#$:#&+%~,]\\S*|\\S*[,*.?!@ ]\\S*|\\+(q|a|o|h|v)\\S*)");

	if (std::regex_match(newNick, incorrect)) {

		std::string msg = ":ft_irc 432 " + _clients.at(getClientIndex(cfd)).getNick() + " " + newNick + " :Erroneus nickname\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
	} else {

		if (isUniqueNick(newNick)) {

			std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " NICK " + newNick + "\r\n";

			for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); std::advance(it, 1)) {
				if ((*it).containSender(cfd)) {
					(*it).broadcast(msg, cfd, false);
				}
			}
			send(cfd, msg.c_str(), msg.length(), 0);
			_clients.at(getClientIndex(cfd)).setNickname(newNick);
		} else {

			std::string msg = ":ft_irc 433 " + _clients.at(getClientIndex(cfd)).getNick() + " " + newNick + " \r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
		}
	}
}

void	Server::nick(int cfd, std::string arg) {
	
	std::vector<std::string>	params;

	parser(arg, params, ' ');
	if (params.at(0) == "NICK") {

		if (params.size() <= 1) {

			std::string msg = ":ft_irc 431 " + _clients.at(getClientIndex(cfd)).getNick() + " :No nickname given\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
			return ;
			
		} else if (_clients.at(getClientIndex(cfd)).getNick().compare(params.at(1)) == 0) {
		} else {
			verifyNick(cfd, params.at(1));
		}
	} else {

		std::string msg = ":ft_irc 421 " + _clients.at(getClientIndex(cfd)).getUser() + " " + params.at(0) + " :Unknown command\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
	}
}
